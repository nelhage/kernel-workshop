from __future__ import print_function

with open('/dev/rot13', 'r+') as fh:
  fh.write("Hello, World")
  print(fh.read())
