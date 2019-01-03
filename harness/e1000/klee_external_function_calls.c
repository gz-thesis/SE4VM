#include <string.h>

#include "net/net.h"            // ./include/net/
#include "net/queue.h"	        // ./include/net/
#include "hw/pci/pci.h"         // ./include/hw/pci/
#include "hw/pci/pci_bus.h"     // ./include/hw/pci/
#include "qemu/timer.h"         // ./include/qemu/
#include "qemu/iov.h"           // ./include/qemu/
#include "qemu-common.h"        // ./include/
#include "qemu/queue.h"	        // ./include/qemu/
#include "qom/object.h"         // ./include/qom/

#define MAX_DEV_SNAPSHOTS  1
#define MAX_INTERFACES 32

// ./qom/object.c
typedef struct InterfaceImpl InterfaceImpl;
typedef struct TypeImpl TypeImpl;

// ./qom/object.c
struct InterfaceImpl
{
    const char *typename;
};

// ./qom/object.c
struct TypeImpl
{
    const char *name;

    size_t class_size;

    size_t instance_size;

    void (*class_init)(ObjectClass *klass, void *data);
    void (*class_base_init)(ObjectClass *klass, void *data);
    void (*class_finalize)(ObjectClass *klass, void *data);

    void *class_data;

    void (*instance_init)(Object *obj);
    void (*instance_post_init)(Object *obj);
    void (*instance_finalize)(Object *obj);

    bool abstract;

    const char *parent;
    TypeImpl *parent_type;

    ObjectClass *class;

    int num_interfaces;
    InterfaceImpl interfaces[MAX_INTERFACES];
};

static Type type_interface;

/* pointers to a device snapshot */
struct dev_snapshots {
    const char * devname;
    int  statesize;
    char * snapshot;
    char * swapspace;  /* tmp buffer */
    const VMStateDescription *vmsd;	 
};

/* a qemu global variable for all DISC device snapshots*/
struct dev_snapshots g_dss[MAX_DEV_SNAPSHOTS];

int g_num_dss = 1;

// ./net/net.c
NICState *qemu_get_nic(NetClientState *nc)
{
    NetClientState *nc0 = nc - nc->queue_index;
    
    return (NICState *)((void *)nc0 - nc->info->size);
}

// ./net/net.c
void *qemu_get_nic_opaque(NetClientState *nc)
{
    NICState *nic = qemu_get_nic(nc);
    
    return nic->opaque;
}

// ./util/iov.c
size_t iov_from_buf(const struct iovec *iov, unsigned int iov_cnt, size_t offset, const void *buf, size_t bytes)
{
    size_t done;
    unsigned int i;
    for (i = 0, done = 0; (offset || done < bytes) && i < iov_cnt; i++) {
        if (offset < iov[i].iov_len) {
            size_t len = MIN(iov[i].iov_len - offset, bytes - done);
            memcpy(iov[i].iov_base + offset, buf + done, len);
            done += len;
            offset = 0;
        } else {
            offset -= iov[i].iov_len;
        }
    }
    assert(offset == 0);
    return done;
}

// ./util/iov.c
size_t iov_to_buf(const struct iovec *iov, const unsigned int iov_cnt, size_t offset, void *buf, size_t bytes)
{
    size_t done;
    unsigned int i;
    for (i = 0, done = 0; (offset || done < bytes) && i < iov_cnt; i++) {
        if (offset < iov[i].iov_len) {
            size_t len = MIN(iov[i].iov_len - offset, bytes - done);
            memcpy(buf + done, iov[i].iov_base + offset, len);
            done += len;
            offset = 0;
        } else {
            offset -= iov[i].iov_len;
        }
    }
    assert(offset == 0);
    return done;
}

// ./util/iov.c
size_t iov_size(const struct iovec *iov, const unsigned int iov_cnt)
{
    
    size_t len;
    unsigned int i;

    len = 0;
    for (i = 0; i < iov_cnt; i++) {
        len += iov[i].iov_len;
    }
    
    return len;
}

// ./hw/pci/pci.c
void pci_set_irq(PCIDevice *pci_dev, int level)
{
    /*
    int intx = pci_intx(pci_dev);
    pci_irq_handler(pci_dev, intx, level);
    */
}

// ./qom/object.c
static GHashTable *type_table_get(void)
{
    static GHashTable *type_table;

    if (type_table == NULL) {
        type_table = g_hash_table_new(g_str_hash, g_str_equal);
    }

    return type_table;
}

// ./qom/object.c
static TypeImpl *type_table_lookup(const char *name)
{
    return g_hash_table_lookup(type_table_get(), name);
}

// ./qom/object.c
static TypeImpl *type_get_by_name(const char *name)
{
    if (name == NULL) {
        return NULL;
    }

    return type_table_lookup(name);
}

// ./qom/object.c
static TypeImpl *type_get_parent(TypeImpl *type)
{
    if (!type->parent_type && type->parent) {
        type->parent_type = type_get_by_name(type->parent);
        g_assert(type->parent_type != NULL);
    }

    return type->parent_type;
}

// ./qom/object.c
static bool type_is_ancestor(TypeImpl *type, TypeImpl *target_type)
{
    assert(target_type);

    /* Check if typename is a direct ancestor of type */
    while (type) {
        if (type == target_type) {
            return true;
        }

        type = type_get_parent(type);
    }

    return false;
}

// ./qom/object.c
ObjectClass *object_get_class(Object *obj)
{
    return obj->class;
}

// ./qom/object.c
Object *object_dynamic_cast(Object *obj, const char *typename)
{
    if (obj && object_class_dynamic_cast(object_get_class(obj), typename)) {
        return obj;
    }

    return NULL;
}

// ./qom/object.c
ObjectClass *object_class_dynamic_cast(ObjectClass *class, const char *typename)
{
    ObjectClass *ret = NULL;
    TypeImpl *target_type;
    TypeImpl *type;

    if (!class) {
        return NULL;
    }

    /* A simple fast path that can trigger a lot for leaf classes.  */
    type = class->type;
    if (type->name == typename) {
        return class;
    }

    target_type = type_get_by_name(typename);
    if (!target_type) {
        /* target class type unknown, so fail the cast */
        return NULL;
    }

    if (type->class->interfaces &&
            type_is_ancestor(target_type, type_interface)) {
        int found = 0;
        GSList *i;

        for (i = class->interfaces; i; i = i->next) {
            ObjectClass *target_class = i->data;

            if (type_is_ancestor(target_class->type, target_type)) {
                ret = target_class;
                found++;
            }
         }

        /* The match was ambiguous, don't allow a cast */
        if (found > 1) {
            ret = NULL;
        }
    } else if (type_is_ancestor(type, target_type)) {
        ret = class;
    }

    return ret;
}

// ./qom/object.c
Object *object_dynamic_cast_assert(Object *obj, const char *typename, const char *file, int line, const char *func)
{   
    trace_object_dynamic_cast_assert(obj ? obj->class->type->name : "(null)", typename, file, line, func);

    #ifdef CONFIG_QOM_CAST_DEBUG
        int i;
        Object *inst;
    
        for (i = 0; obj && i < OBJECT_CLASS_CAST_CACHE; i++) {
            if (obj->class->object_cast_cache[i] == typename) {
                goto out;
            }
        }
    
        inst = object_dynamic_cast(obj, typename);
    
        if (!inst && obj) {
            fprintf(stderr, "%s:%d:%s: Object %p is not an instance of type %s\n", file, line, func, obj, typename);
            abort();
        }
    
        assert(obj == inst);
    
        if (obj && obj == inst) {
            for (i = 1; i < OBJECT_CLASS_CAST_CACHE; i++) {
                obj->class->object_cast_cache[i - 1] = obj->class->object_cast_cache[i];
            }
            obj->class->object_cast_cache[i - 1] = typename;
        }
    
        out:
    #endif
    
    return obj;
}

// ./util/module.c
void register_module_init(void (*fn)(void), module_init_type type)
{
    
}

// ./qemu-timer.c
void timer_mod(QEMUTimer *ts, int64_t expire_time)
{
    //timer_mod_ns(ts, expire_time * ts->scale);
}

// ./qemu-timer.c
int64_t qemu_clock_get_ns(QEMUClockType type)
{
    int64_t now, last;
    /*
    QEMUClock *clock = qemu_clock_ptr(type);

    switch (type) {
        case QEMU_CLOCK_REALTIME:
            return get_clock();
        default:
        case QEMU_CLOCK_VIRTUAL:
            if (use_icount) {
                return cpu_get_icount();
            } else {
                return cpu_get_clock();
            }
        case QEMU_CLOCK_HOST:
            now = get_clock_realtime();
            last = clock->last;
            clock->last = now;
            if (now < last) {
                notifier_list_notify(&clock->reset_notifiers, &now);
            }
        return now;
    }
    */
    return now;
}

// ./util/notify.c
void notifier_list_notify(NotifierList *list, void *data)
{

}

// ./exec.c
bool address_space_rw(AddressSpace *as, hwaddr addr, uint8_t *buf, int len, bool is_write)
{
    bool error = false;
    
    return error;
}

// ./net/queue.c
struct NetPacket {
    QTAILQ_ENTRY(NetPacket) entry;
    NetClientState *sender;
    unsigned flags;
    int size;
    NetPacketSent *sent_cb;
    uint8_t data[0];
};

// ./net/queue.c
struct NetQueue {
    void *opaque;
    uint32_t nq_maxlen;
    uint32_t nq_count;

    QTAILQ_HEAD(packets, NetPacket) packets;

    unsigned delivering : 1;
};

// ./net/net.c
NetClientState *qemu_get_subqueue(NICState *nic, int queue_index)
{
    return nic->ncs + queue_index;
}

// ./net/net.c
NetClientState *qemu_get_queue(NICState *nic)
{
    return qemu_get_subqueue(nic, 0);
}

// ./net/net.c
ssize_t qemu_deliver_packet(NetClientState *sender, unsigned flags, const uint8_t *data, size_t size, void *opaque)
{
    NetClientState *nc = opaque;
    ssize_t ret;

    if (nc->link_down) {
        return size;
    }

    if (nc->receive_disabled) {
        return 0;
    }

    if (flags & QEMU_NET_PACKET_FLAG_RAW && nc->info->receive_raw) {
        ret = nc->info->receive_raw(nc, data, size);
    } else {
        ret = nc->info->receive(nc, data, size);
    }

    if (ret == 0) {
        nc->receive_disabled = 1;
    };

    return ret;
}

// ./net/queue.c
static ssize_t qemu_net_queue_deliver(NetQueue *queue, NetClientState *sender, unsigned flags, const uint8_t *data, size_t size)
{
    ssize_t ret = -1;

    queue->delivering = 1;
    ret = qemu_deliver_packet(sender, flags, data, size, queue->opaque);
    queue->delivering = 0;

    return ret;
}

// ./net/queue.c
bool qemu_net_queue_flush(NetQueue *queue)
{
    while (!QTAILQ_EMPTY(&queue->packets)) {
        NetPacket *packet;
        int ret;

        packet = QTAILQ_FIRST(&queue->packets);
        QTAILQ_REMOVE(&queue->packets, packet, entry);
        queue->nq_count--;

        ret = qemu_net_queue_deliver(queue, packet->sender, packet->flags, packet->data, packet->size);
        if (ret == 0) {
            queue->nq_count++;
            QTAILQ_INSERT_HEAD(&queue->packets, packet, entry);
            return false;
        }

        if (packet->sent_cb) {
            packet->sent_cb(packet->sender, ret);
        }

        g_free(packet);
    }
    return true;
}

// ./net/queue.c
static void qemu_net_queue_append(NetQueue *queue, NetClientState *sender, unsigned flags, const uint8_t *buf, size_t size, NetPacketSent *sent_cb)
{
    NetPacket *packet;

    if (queue->nq_count >= queue->nq_maxlen && !sent_cb) {
        return; /* drop if queue full and no callback */
    }
    packet = g_malloc(sizeof(NetPacket) + size);
    packet->sender = sender;
    packet->flags = flags;
    packet->size = size;
    packet->sent_cb = sent_cb;
    memcpy(packet->data, buf, size);

    queue->nq_count++;
    QTAILQ_INSERT_TAIL(&queue->packets, packet, entry);
}

// ./net/net.c
int qemu_can_send_packet(NetClientState *sender)
{
    if (!sender->peer) {
        return 1;
    }

    if (sender->peer->receive_disabled) {
        return 0;
    } else if (sender->peer->info->can_receive &&
               !sender->peer->info->can_receive(sender->peer)) {
        return 0;
    }
    return 1;
}

// ./net/queue.c
ssize_t qemu_net_queue_send(NetQueue *queue, NetClientState *sender, unsigned flags, const uint8_t *data, size_t size, NetPacketSent *sent_cb)
{
    ssize_t ret;

    if (queue->delivering || !qemu_can_send_packet(sender)) {
        qemu_net_queue_append(queue, sender, flags, data, size, sent_cb);
        return 0;
    }

    ret = qemu_net_queue_deliver(queue, sender, flags, data, size);
    if (ret == 0) {
        qemu_net_queue_append(queue, sender, flags, data, size, sent_cb);
        return 0;
    }

    qemu_net_queue_flush(queue);

    return ret;
}

// ./net/net.c
static ssize_t qemu_send_packet_async_with_flags(NetClientState *sender, unsigned flags, const uint8_t *buf, int size, NetPacketSent *sent_cb)
{
    NetQueue *queue;

    #ifdef DEBUG_NET
        printf("qemu_send_packet_async:\n");
        hex_dump(stdout, buf, size);
    #endif

    if (sender->link_down || !sender->peer) {
        return size;
    }

    queue = sender->peer->incoming_queue;

    return qemu_net_queue_send(queue, sender, flags, buf, size, sent_cb);
}

// ./net/net.c
ssize_t qemu_send_packet_async(NetClientState *sender, const uint8_t *buf, int size, NetPacketSent *sent_cb)
{
    return qemu_send_packet_async_with_flags(sender, QEMU_NET_PACKET_FLAG_NONE, buf, size, sent_cb);
}

void qemu_send_packet(NetClientState *nc, const uint8_t *buf, int size)
{
    qemu_send_packet_async(nc, buf, size, NULL);
}

// ./net/checksum.c
uint32_t net_checksum_add_cont(int len, uint8_t *buf, int seq)
{
    uint32_t sum = 0;
    int i;

    for (i = seq; i < seq + len; i++) {
        if (i & 1) {
            sum += (uint32_t)buf[i - seq];
        } else {
            sum += (uint32_t)buf[i - seq] << 8;
        }
    }
    return sum;
}

// ./net/hub.c
typedef struct NetHub {
    int id;
    QLIST_ENTRY(NetHub) next;
    int num_ports;
    QLIST_HEAD(, NetHubPort) ports;
} NetHub;

// ./net/hub.c
typedef struct NetHubPort {
    NetClientState nc;
    QLIST_ENTRY(NetHubPort) next;
    NetHub *hub;
    int id;
} NetHubPort;

// ./net/hub.c
bool net_hub_flush(NetClientState *nc)
{
    NetHubPort *port;
    NetHubPort *source_port = DO_UPCAST(NetHubPort, nc, nc);
    int ret = 0;

    QLIST_FOREACH(port, &source_port->hub->ports, next) {
        if (port != source_port) {
            ret += qemu_net_queue_flush(port->nc.incoming_queue);
        }
    }
    return ret ? true : false;
}

// ./net/checksum.c
uint16_t net_checksum_finish(uint32_t sum)
{
    while (sum>>16)
    sum = (sum & 0xFFFF)+(sum >> 16);
    return ~sum;
}

// ./net/net.c
void qemu_format_nic_info_str(NetClientState *nc, uint8_t macaddr[6])
{
    snprintf(nc->info_str, sizeof(nc->info_str), "model=%s,macaddr=%02x:%02x:%02x:%02x:%02x:%02x", nc->model, macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
}

// ./net/net.c
void qemu_flush_queued_packets(NetClientState *nc)
{
    nc->receive_disabled = 0;

    if (nc->peer && nc->peer->info->type == NET_CLIENT_OPTIONS_KIND_HUBPORT) {
        if (net_hub_flush(nc->peer)) {
            qemu_notify_event();
        }
    }
    if (qemu_net_queue_flush(nc->incoming_queue)) {
        /* We emptied the queue successfully, signal to the IO thread to repoll
         * the file descriptor (for tap, for example).
         */
        qemu_notify_event();
    }
}

// ./main-loop.c
void qemu_notify_event(void)
{

}

// ./hw/pci/pci.c
struct PCIHostBus {
    int domain;
    struct PCIBus *bus;
    QLIST_ENTRY(PCIHostBus) next;
};
static QLIST_HEAD(, PCIHostBus) host_buses;

// ./hw/pci/pci.c
PCIBus *pci_find_root_bus(int domain)
{
    struct PCIHostBus *host;

    QLIST_FOREACH(host, &host_buses, next) {
        if (host->domain == domain) {
            return host->bus;
        }
    }

    return NULL;
}

/* 
 * DISC Function for snapshot
 */
void device_load_state(const VMStateDescription *vmsd, void *opaque, void *swapspace){

    VMStateField *field = vmsd->fields; 

    /* The following piece of code is from 
        vmstate_save_state() in savevm.c
    */
    
    while(field->name) {
        if (!field->field_exists || field->field_exists(opaque, vmsd->version_id)) {
            printf("%s\n",field->name);
            void *base_addr = opaque + field->offset;
            void *swap_base_addr = swapspace + field->offset;
            int i, n_elems = 1;
            int size = field->size;

            if (field->flags & VMS_VBUFFER) {
                size = *(int32_t *)(swapspace + field->size_offset);
                if (field->flags & VMS_MULTIPLY) {
                        size *= field->size;
                }
            }

            if (field->flags & VMS_ARRAY) {
                n_elems = field->num;
            } else if (field->flags & VMS_VARRAY_INT32) {
                n_elems = *(int32_t *)(swapspace + field->num_offset);
            } else if (field->flags & VMS_VARRAY_UINT32) {
                n_elems = *(uint32_t *)(swapspace + field->num_offset);
            } else if (field->flags & VMS_VARRAY_UINT16) {
                n_elems = *(uint16_t *)(swapspace + field->num_offset);
            } else if (field->flags & VMS_VARRAY_UINT8) {
                n_elems = *(uint8_t *)(swapspace + field->num_offset);
            }
            
            if (field->flags & VMS_POINTER) {
                swap_base_addr = *(void **)swap_base_addr + field->start;
                printf("ALERT: VMS_POINTER\n");
            }

            for (i = 0; i < n_elems; i++) {
                void *addr = base_addr + size * i;
                void *swap_addr = swap_base_addr + size * i;

                if (field->flags & VMS_ARRAY_OF_POINTER) {
                    swap_addr = *(void **)swap_addr;
                    printf("ALERT: VMS_ARRAY_OF_POINTER\n");
                }
                
                if (field->flags & VMS_STRUCT) {
                    //vmstate_save_state(f, field->vmsd, addr); --> from savevm.c
                    device_load_state(field->vmsd, addr, swap_addr);
                } else {
                    //field->info->put(f, addr, size);
                    /* Need to check the pointer type
                    * before calculting addr by pointer
                    * arithmatic 
                    */
                    //memcpy(addr, swapspace+(addr-opaque),size);
                    memcpy(addr, swap_addr, size);
                }
            }
        } //if
        field++;
    }//while
}

/* 
 * DISC Function: 
 * 	set dev stat to init snapshot 
 *	except keeping those in vmsd unchanged 
 * 
 *  opaque points to the device state that is 
 * 	to be flipped to init state except vmsd fields.
 *
 *	device name is provided as parameter
 * 	This can be changed to device_ID and Vendor_ID
 */
void device_nap_reset(const char *name, void *opaque)
{
    int i = 0;  

    while (i < g_num_dss) {
        if (strcmp(name, g_dss[i].devname) == 0) {
            const VMStateDescription *vmsd = g_dss[i].vmsd;	    	
            int sz = g_dss[i].statesize;		
            void *swapspace = g_dss[i].swapspace;

            /* Fix Me: 
             * Currently this has two memcpy + vmsd scan .
             * This could be potentially done by one copy.
             */ 

            /* Copy the dev state to the tmp buffer */
            memcpy(swapspace, opaque, sz);
                    
            /* Copy the snapshot to device state */		
            memcpy(opaque, g_dss[i].snapshot, sz);

            /* restore the vmsd states from the tmp buffer */
            device_load_state(vmsd, opaque, swapspace);

            /* end the search in snapshot */
            break;
        } else 
            i++;
    }

    if (i==g_num_dss) 
        printf("Did not find device %s snapshot.\n", name);

    return;
}

/* 
 * DISC Function
 */
/* 
void find_device_state_by_name(const char *dev_name){
    printf("====>in find_device_state_by_name: %s\n", dev_name);
    int i;
    printf("#############################\n");
    PCIBus *pci_bus = pci_find_root_bus(0);
    if(pci_bus != NULL){
        for (i = 0; i < ARRAY_SIZE(pci_bus->devices); ++i) {
            if (pci_bus->devices[i] != NULL && strcmp(pci_bus->devices[i]->name, dev_name) == 0) {
                printf("i=%d\n", i);
                printf("%s\n", pci_bus->devices[i]->name);
                //invoke device nap reset here by passing pci_bus->devices[i].
                device_nap_reset(pci_bus->devices[i]->name, pci_bus->devices[i]);
            }else if(pci_bus->devices[i] != NULL){
                printf("i=%d\n", i);
                printf("Other device=%s\n", pci_bus->devices[i]->name);
            }
        }
    }
    printf("#############################\n");
}
*/

void flip(const char *dev_name, PCIDevice *d)
{
    device_nap_reset(dev_name, d);
    //find_device_state_by_name(dev_name);
}
