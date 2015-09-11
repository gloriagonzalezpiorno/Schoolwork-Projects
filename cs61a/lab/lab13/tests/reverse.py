test = {
  'name': 'reverse',
  'points': 1,
  'suites': [
    {
      'cases': [
        {
          'code': r"""
          scm> (reverse (list 1 2 3))
          (3 2 1)
          """,
          'hidden': False,
          'locked': False
        },
        {
          'code': r"""
          scm> (reverse (list 'a 'b 'c 'd 'e 'f))
          c79abb4b527949b0f447621f57d7f7b5
          # locked
          """,
          'hidden': False,
          'locked': True
        },
        {
          'code': r"""
          scm> (reverse '(1))
          eaefd4a7b5c381c412c645d17a64da54
          # locked
          """,
          'hidden': False,
          'locked': True
        },
        {
          'code': r"""
          scm> (reverse '())
          02348d6f35c2f9940ab9056f05d6a9eb
          # locked
          """,
          'hidden': False,
          'locked': True
        }
      ],
      'scored': True,
      'setup': r"""
      scm> (load 'lab13)
      """,
      'teardown': '',
      'type': 'scheme'
    }
  ]
}