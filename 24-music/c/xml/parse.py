#!/usr/bin/env python3

import music21

# get some music.

#fn = 'Sail On, Boys v2.musicxml'
#fn = 'the-deep-river-samba-traditionalish.musicxml'
fn = 'super-mario-bros-main-theme.musicxml'

score = music21.converter.parse(fn).expandRepeats()

# This score was arranged for clarinet choir (Bb, Alto, Bass, Contra-alto, Contrabass),
# so all of the parts need to be transposed.
#
# Bb clarinet: down a major second
# Alto clarinet down a major sixth
# Bass clarinet: down an octave and a major second
# Contraalto clarinet: down an octave and a major sixth
# Contrabass clarinet: down two octaves and a major second
# 
# Although music21 has an instrumental translation lookup, music21's support for
# clarinet-family instruments is limited (only Bb clarinet and bass clarinet). So
# we have to do this the hard way...

tr = { 'Bb Clarinet': music21.interval.Interval('M-2'),
       'Eb Alto': music21.interval.Interval('M-6'),
       'Bb Bass': music21.interval.Interval('M-9'),
       'Eb Contra-alto': music21.interval.Interval('M-13'),
       'Bb Contrabass': music21.interval.Interval('M-16'),
       'Soprano Saxophone': music21.interval.Interval('M-2'),
       'Alto Saxophone': music21.interval.Interval('M-6'),
       'Tenor Saxophone': music21.interval.Interval('M-9'),
       'Baritone Saxophone': music21.interval.Interval('M-13'),
       'Piano': 0
     }

parts = score.parts

# stash of notes (format [start-time, pitch, duration])
notes = []

for pno,part in enumerate(parts):
  inst = part.getInstrument()
  iname = inst.partName.split(' ')
  if len(iname) >= 2:
    iname = ' '.join(iname.split(' ')[:2])
  else:
    iname = iname[0]
  # Eb instruments not properly transposed
  print(f'Inst: {iname}; transposition: {tr[iname]}')
  for el in part.flatten():
    #print(f'Element: {el}')
    if isinstance(el,music21.note.Note):
      #print(f' Note: offset {el.offset}, pitch {el.nameWithOctave} duration {el.duration}')
      # fix the *(@&$# note
      if tr[iname] != 0:
        el.transpose(tr[iname],inPlace=True) # whee!
      # enter start and end times (numerical)
      el.nstart = 100.0 * float(el.offset)
      el.nend = 100.0 * float(el.offset + el.duration.quarterLength)
      notes.append(el)
      #print(f'{float(el.offset)},{el.nameWithOctave},{el.duration.type}')

notes.sort(key = lambda x: float(x.offset))

for n in notes:
  print(f'{int(n.nstart)},{n.nameWithOctave},{int(n.nend)}')
