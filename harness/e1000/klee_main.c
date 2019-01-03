#include "e1000.c"
#include "klee_external_function_calls.c"

#define ITERATION    1

bool kvm_allowed;
//int use_icount;
//int use_rt_clock;

const VMStateInfo vmstate_info_bool = {
    .name = "bool",
};
const VMStateInfo vmstate_info_int8 = {
    .name = "int8",
};
const VMStateInfo vmstate_info_int16 = {
    .name = "int16",
};
const VMStateInfo vmstate_info_int32 = {
    .name = "int32",
};
const VMStateInfo vmstate_info_uint8 = {
    .name = "uint8",
};
const VMStateInfo vmstate_info_uint16 = {
    .name = "uint16",
};
const VMStateInfo vmstate_info_uint32 = {
    .name = "uint32",
};
const VMStateDescription vmstate_pci_device = {
    .name = "PCIDevice",
    .version_id = 2,
    .minimum_version_id = 1,
    .minimum_version_id_old = 1,
};
const VMStateInfo vmstate_info_buffer = {
    .name = "buffer",
};
const VMStateInfo vmstate_info_unused_buffer = {
    .name = "unused_buffer",
};
PropertyInfo qdev_prop_macaddr = {
    .name  = "macaddr",
};
PropertyInfo qdev_prop_vlan = {
    .name  = "vlan",
};
PropertyInfo qdev_prop_netdev = {
    .name  = "netdev",
};
PropertyInfo qdev_prop_uint32 = {
    .name  = "uint32",
};
PropertyInfo qdev_prop_int32 = {
    .name  = "int32",
};
PropertyInfo qdev_prop_bit = {
    .name  = "bit",
};

QEMUTimerListGroup main_loop_tlg;

int main() {

    //klee_make_symbolic(&use_icount, sizeof(use_icount), "use_icount");
    //klee_make_symbolic(&use_rt_clock, sizeof(use_rt_clock), "use_rt_clock");
    
    E1000State *s;
    NICState *nic;
    NetClientInfo *info = &net_e1000_info;

    s = malloc(sizeof(E1000State));

    nic = malloc(info->size + sizeof(NetClientState));
    nic->ncs = (void *)nic + info->size;
    nic->opaque = (void *)s;

    nic->ncs[0].queue_index = 0;
    nic->ncs[0].info = info;
    // used when retrieve E1000State from NICState
    s->nic = nic;

    // used when retrieve NICState from NetClientState

    int link_down;
    klee_make_symbolic(&link_down, sizeof(link_down), "link_down");
    memcpy(&nic->ncs[0].link_down, &link_down, sizeof(int));
    
    bool mit_timer_on;
    bool mit_irq_level;
    klee_make_symbolic(&mit_timer_on, sizeof(mit_timer_on), "mit_timer_on");
    klee_make_symbolic(&mit_irq_level, sizeof(mit_irq_level), "mit_irq_level");
    memcpy(&s->mit_timer_on, &mit_timer_on, sizeof(mit_timer_on));
    memcpy(&s->mit_irq_level, &mit_irq_level, sizeof(mit_irq_level));

    uint32_t mac_reg[0x8000] = { 0 };
    // make individual registers instead of the whole array to reduce the workload of constraint solver
    //klee_make_symbolic(mac_reg, sizeof(mac_reg), "mac_reg");
    uint32_t sym_val, sym_arr_32[32], sym_arr_128[128];

    klee_make_symbolic(&sym_val, sizeof(uint32_t), "STATUS");
    memcpy(&mac_reg[STATUS], &sym_val, sizeof(sym_val));

    klee_make_symbolic(&sym_val, sizeof(uint32_t), "ICR");
    memcpy(&mac_reg[ICR], &sym_val, sizeof(sym_val));

    klee_make_symbolic(&sym_val, sizeof(uint32_t), "ICS");
    memcpy(&mac_reg[ICS], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "IMS");
    memcpy(&mac_reg[IMS], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "TADV");
    memcpy(&mac_reg[TADV], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "RDTR");
    memcpy(&mac_reg[RDTR], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "RADV");
    memcpy(&mac_reg[RADV], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "ITR");
    memcpy(&mac_reg[ITR], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "CTRL");
    memcpy(&mac_reg[CTRL], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "RCTL");
    memcpy(&mac_reg[RCTL], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "MDIC");
    memcpy(&mac_reg[MDIC], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "EERD");
    memcpy(&mac_reg[EERD], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "TPT");
    memcpy(&mac_reg[TPT], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "GPTC");
    memcpy(&mac_reg[GPTC], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "TOTL");
    memcpy(&mac_reg[TOTL], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "TOTH");
    memcpy(&mac_reg[TOTH], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "TDBAH");
    memcpy(&mac_reg[TDBAH], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "TDBAL");
    memcpy(&mac_reg[TDBAL], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "TDH");
    memcpy(&mac_reg[TDH], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "TCTL");
    memcpy(&mac_reg[TCTL], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "TDT");
    memcpy(&mac_reg[TDT], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "TDLEN");
    memcpy(&mac_reg[TDLEN], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "RDH");
    memcpy(&mac_reg[RDH], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "RDT");
    memcpy(&mac_reg[RDT], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "RDLEN");
    memcpy(&mac_reg[RDLEN], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "RDBAH");
    memcpy(&mac_reg[RDBAH], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "RDBAL");
    memcpy(&mac_reg[RDBAL], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "GPRC");
    memcpy(&mac_reg[GPRC], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "TPR");
    memcpy(&mac_reg[TPR], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "TORL");
    memcpy(&mac_reg[TORL], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "TORH");
    memcpy(&mac_reg[TORH], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(&sym_val, sizeof(uint32_t), "VET");
    memcpy(&mac_reg[VET], &sym_val, sizeof(sym_val));
    
    klee_make_symbolic(sym_arr_32, sizeof(sym_arr_32), "RA");
    memcpy(&mac_reg[RA], sym_arr_32, sizeof(sym_arr_32));
    
    klee_make_symbolic(sym_arr_128, sizeof(sym_arr_128), "MTA");
    memcpy(&mac_reg[MTA], sym_arr_128, sizeof(sym_arr_128));
    
    klee_make_symbolic(sym_arr_128, sizeof(sym_arr_128), "VFTA");
    memcpy(&mac_reg[VFTA], sym_arr_128, sizeof(sym_arr_128));

    memcpy(s->mac_reg, mac_reg, sizeof(mac_reg));

    uint32_t compat_flags;
    uint32_t mit_ide;
    klee_make_symbolic(&compat_flags, sizeof(compat_flags), "compat_flags");
    klee_make_symbolic(&mit_ide, sizeof(mit_ide), "mit_ide");
    memcpy(&s->compat_flags, &compat_flags, sizeof(compat_flags));
    memcpy(&s->mit_ide, &mit_ide, sizeof(mit_ide));
   
    // these variables are set in one path of e1000_mmio_write
    uint32_t rxbuf_size;
    uint32_t rxbuf_min_shift;
    uint64_t val;
    klee_make_symbolic(&val, sizeof(val), "val");
    rxbuf_min_shift = ((val / E1000_RCTL_RDMTS_QUAT) & 3) + 1;
    rxbuf_size = rxbufsize(val);
    memcpy(&s->rxbuf_size, &rxbuf_size, sizeof(rxbuf_size));
    memcpy(&s->rxbuf_min_shift, &rxbuf_min_shift, sizeof(rxbuf_min_shift));

    // TODO:we want to simulate the init function of e1000
    //pci_e1000_init();

    int deviceEntry;
    uint8_t buf[60];
    size_t size;
    klee_make_symbolic(&deviceEntry, sizeof(deviceEntry), "deviceEntry");
    klee_make_symbolic(buf, sizeof(buf), "buf");
    klee_make_symbolic(&size, sizeof(size), "size");

    hwaddr addr;
    unsigned unsigned_size;
    klee_make_symbolic(&addr, sizeof(addr), "addr");
    klee_make_symbolic(&val, sizeof(val), "val");
    klee_make_symbolic(&unsigned_size, sizeof(unsigned_size), "unsigned_size");

    //int i;   
    //for(i = 0; i < ITERATION; i++) {
    //    switch(deviceEntry) {
    //        case 1:
    //            e1000_set_link_status(ncState);
    //            break;
    //        case 2:
    //            e1000_receive(ncState, (const uint8_t *)buf, size);
    //            break;
    //        case 3:
    //            e1000_mmio_read((void *)&s, addr, unsigned_size);
    //            break;
    //        case 4:
    //            e1000_mmio_write((void *)&s, addr, val, unsigned_size);
    //            break;
    //        default:
    //            break;
    //    }
    //}
    
    PCIDevice *d = PCI_DEVICE(s);
    char *dev_name = "e1000";
    
    g_dss[0].devname = dev_name;
    g_dss[0].statesize = sizeof(PCIDevice);
    g_dss[0].snapshot = NULL;
    g_dss[0].swapspace = NULL;
    g_dss[0].vmsd = &vmstate_e1000;
    
    e1000_receive(&nic->ncs[0], (const uint8_t *)buf, size);
    e1000_mmio_write((void *)s, addr, val, unsigned_size);
    flip(dev_name, d);
    e1000_receive(&nic->ncs[0], (const uint8_t *)buf, size);

    return 0;
}
