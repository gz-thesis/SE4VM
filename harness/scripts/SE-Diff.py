#!/usr/bin/python
import sys,os
from subprocess import call,check_output   # run shell commands

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

def pred(content):
  return bcolors.FAIL+content+bcolors.ENDC


last="klee-last"
llvm_command = ['llvm-gcc']
llvm_options = ['--emit-llvm','-c','-g']
klee_command = ['klee']
klee_options = ['--libc=uclibc', '-write-pcs']
md5sum = 'md5sum {DIR}/*.pc'
f1 = ''
f2 = ''

def generate(filename):
  command = llvm_command + llvm_options 
  command.append(filename)
  obj = filename.replace('.c', '.bc')
  command.append('-o')
  command.append(obj)
  with open(os.devnull, "w") as fnull:
    ret = call(command, stdout=fnull, stderr=fnull) 
  return obj if ret==0 else None


def execute(filename):
  command = klee_command + klee_options
  command.append(filename)
  with open(os.devnull, "w") as fnull:
    ret = call(command, stdout=fnull, stderr=fnull)
  return ret


def loadhash(dirname):
  command = md5sum.replace('{DIR}', dirname)
  a = check_output(command, shell=True).split()
  b = {a[i]: a[i+1] for i in range(0, len(a), 2)}
  return b


def compare(dict1, dict2):
  before = list(dict1.keys())
  after = list(dict2.keys())

  before.sort()
  after.sort()

  idx_before = 0
  idx_after = 0
  l0 = []
  l1 = []
  l2 = []

  while idx_before < len(before) and idx_after < len(after):
    cur_before = before[idx_before]
    cur_after = after[idx_after]
    if cur_before == cur_after:
      l0.append([before[idx_before],after[idx_after]])
      idx_before += 1
      idx_after += 1
      continue
    if cur_before < cur_after:
      l1.append(before[idx_before])
      idx_before += 1
    else:
      l2.append(after[idx_after])
      idx_after += 1

  # be careful here, merge the remainder into the corresponding list
  l1 = l1 + before[idx_before:]
  l2 = l2 + after[idx_after:]
  return [l0,l1,l2]


def lookup(result, dict1, dict2):
  print '\nPaths only generated from '+pred(f1)+':'
  for i,item in enumerate(result[1]):
    print str(i+1) + "\t:   "+dict1.get(item)
  print '\nPaths only generated from '+pred(f2)+':'
  for i,item in enumerate(result[2]):
    print str(i+1) + "\t:   "+dict2.get(item)
  print '\nPaths in common:'
  for i,item in enumerate(result[0]):
    print str(i+1) + "\t:   "+dict1.get(item[0])
    print "\t:   "+dict2.get(item[1])


def usage(idx):
  if idx == 0:       # files can not be found
    print 'files can not be found'
  elif idx == 1:     # can not be compiled with llvm-gcc
    print 'files can not be compiled into llvm bytecode'
  elif id == 2:      # klee error
    print 'failed to run the bytecode with klee'
  else:              # other
    print 'error'
  exit()


if __name__ == '__main__':
  # get the file name to be tested
  try:
    f1 = sys.argv[1]
    f2 = sys.argv[2]
  except:
    usage(0)

  # compile and execute the first file
  print 'Compiling '+pred(f1)+' into llvm bytecode...',
  bc1 = generate(f1)
  print 'done\nExecuting '+pred(f1)+' with klee...',
  res = execute(bc1)
  print 'done'
  # get the directory name of the output
  dir1 = os.path.relpath(os.readlink(last))

  # compile and execute the second file
  print 'Compiling '+pred(f2)+' into llvm bytecode...',
  bc2 = generate(f2)
  print 'done\nExecuting '+pred(f2)+' with klee...',
  res = execute(bc2)
  print 'done'
  # get the directory name of the output
  dir2 = os.path.relpath(os.readlink(last))

  # calculate the hash and load the hash into dictionary
  print 'Comparing the two generated result sets...',
  dt1 = loadhash(dir1)
  dt2 = loadhash(dir2)

  # compare and print the result
  diff = compare(dt1, dt2)
  print 'done'

  # lookup
  lookup(diff, dt1, dt2)
