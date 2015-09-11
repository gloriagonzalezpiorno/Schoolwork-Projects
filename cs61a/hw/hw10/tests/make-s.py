test = {
  'name': 'make-s',
  'points': 1,
  'suites': [
    {
      'cases': [
        {
          'code': r"""
          scm> (stream-to-list (make-s) 20)
          (1 2 3 4 5 6 8 9 10 12 15 16 18 20 24 25 27 30 32 36)
          """,
          'hidden': False,
          'locked': False
        }
      ],
      'scored': True,
      'setup': r"""
      scm> (load 'hw10)
      """,
      'teardown': '',
      'type': 'scheme'
    }
  ]
}