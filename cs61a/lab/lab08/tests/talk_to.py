test = {
  'name': 'Talk to',
  'points': 0,
  'suites': [
    {
      'cases': [
        {
          'code': r"""
          >>> robert = Character('Robert', 'Have to run for lecture!')
          >>> sather_gate = Place('Sather Gate', 'You are at Sather Gate', [robert], [])
          >>> me = Player('player', sather_gate)
          >>> me.talk_to(robert)
          Person has to be a string.
          >>> me.talk_to('Robert')
          Robert says: Have to run for lecture!
          >>> me.talk_to('Albert')
          Albert is not here.
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