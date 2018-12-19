from intelhex import IntelHex

ih = IntelHex()
ih.fromfile('sound-to-mid_master.hex', format = 'hex')
pydict = ih.todict()

hash = 0
for i in range(len(pydict)):
  hash ^= pydict[i]
  #print(hash, pydict[i])

print(hex(hash))

