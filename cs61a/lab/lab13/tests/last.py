test = {
  'name': 'last',
  'points': 1,
  'suites': [
    {
      'cases': [
        {
          'code': r"""
          scm> (last (list 1 2 3))
          39b37cb324f85b7eed07d86b108c34f5
          # locked
          """,
          'hidden': False,
          'locked': True
        },
        {
          'code': r"""
          scm> (last (list 'a 'b 'c 'd 'e 'f))
          f2d0f2bf7263f3382590a1061583aa2e
          # locked
          """,
          'hidden': False,
          'locked': True
        },
        {
          'code': r"""
          scm> (last '(1))
          7d8d680b0d4d5098bec563bb33a0782e
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