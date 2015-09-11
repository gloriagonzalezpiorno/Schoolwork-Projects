test = {
  'name': 'cycle',
  'points': 1,
  'suites': [
    {
      'cases': [
        {
          'code': r"""
          scm> (cycle nil)
          02348d6f35c2f9940ab9056f05d6a9eb
          # locked
          """,
          'hidden': False,
          'locked': True
        },
        {
          'code': r"""
          scm> (stream-to-list (cycle '(1)) 10)
          01762b7c02bf58109b1ceb6e5b13c27f
          # locked
          """,
          'hidden': False,
          'locked': True
        },
        {
          'code': r"""
          scm> (stream-to-list (cycle '(1 2 3)) 10)
          8a0ee0b5d367ffb0bfd292de83e95c65
          # locked
          """,
          'hidden': False,
          'locked': True
        }
      ],
      'scored': True,
      'setup': r"""
      scm> (load 'lab13_extra)
      """,
      'teardown': '',
      'type': 'scheme'
    }
  ]
}