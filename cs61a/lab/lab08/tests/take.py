test = {
  'name': 'Take',
  'points': 0,
  'suites': [
    {
      'cases': [
        {
          'code': r"""
          >>> hotdog = Thing('Hotdog', 'A hot looking hotdog')
          >>> gbc = Place('GBC', 'You are at Golden Bear Cafe', [], [hotdog])
          >>> me = Player('Player', gbc)
          >>> me.backpack
          []
          >>> me.take(hotdog)
          Thing should be a string.
          >>> me.take('dog')
          dog is not here
          >>> me.take('Hotdog')
          Player takes the Hotdog
          >>> me.take('Hotdog')
          Hotdog is not here
          >>> len(me.backpack)
          1
          """,
          'hidden': False,
          'locked': False
        }
      ],
      'scored': True,
      'setup': r"""
      from data import *
      """,
      'teardown': '',
      'type': 'doctest'
    }
  ]
}