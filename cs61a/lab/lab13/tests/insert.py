test = {
  'name': 'insert',
  'points': 1,
  'suites': [
    {
      'cases': [
        {
          'code': r"""
          scm> (insert 4 (list 1 2 3))
          aa8572b2ec3849b5dd54b892f165898f
          # locked
          """,
          'hidden': False,
          'locked': True
        },
        {
          'code': r"""
          scm> (insert 4 (list 1 2 6 7))
          713a34d5a5e08eb833b89458bfcf0760
          # locked
          """,
          'hidden': False,
          'locked': True
        },
        {
          'code': r"""
          scm> (insert 0 '(1))
          fb44af0503c4eff288b3b8a3e57399b0
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