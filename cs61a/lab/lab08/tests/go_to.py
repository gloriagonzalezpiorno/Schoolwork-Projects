test = {
  'name': 'Go to',
  'points': 0,
  'suites': [
    {
      'cases': [
        {
          'code': r"""
          >>> sather_gate = Place('Sather Gate', 'You are at Sather Gate', [], [])
          >>> gbc = Place('GBC', 'You are at Golden Bear Cafe', [], [])
          >>> sather_gate.add_exits([gbc])
          >>> gbc.add_exits([sather_gate])
          >>> me = Player('player', sather_gate)
          >>> me.go_to('GBC')
          You are at Golden Bear Cafe
          >>> me.place.name
          'GBC'
          >>> me.go_to('GBC')
          Can't go to GBC from GBC.
          Try looking around to see where to go.
          >>> me.go_to('Sather Gate')
          You are at Sather Gate
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