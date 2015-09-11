test = {
  'name': 'stream-filter',
  'points': 1,
  'suites': [
    {
      'cases': [
        {
          'code': r"""
          scm> (define a (cons-stream 1 (cons-stream 2 (cons-stream 3 nil))))
          bd36b05380093e67fd716faf8b2a5fd7
          # locked
          scm> (stream-to-list (stream-filter a even?) 10)
          151bbe35401b6510408fb5341cdcb36a
          # locked
          """,
          'hidden': False,
          'locked': True
        },
        {
          'code': r"""
          scm> (define a (cons-stream 3 (cons-stream 6 (cons-stream 8 (cons-stream 4 (cons-stream 5 (cons-stream 3 nil)))))))
          bd36b05380093e67fd716faf8b2a5fd7
          # locked
          scm> (stream-to-list (stream-filter a odd?) 10)
          3793848b222586e12e78d99d03c53b09
          # locked
          """,
          'hidden': False,
          'locked': True
        },
        {
          'code': r"""
          scm> (define both (cons-stream 1 (cons-stream 2 both)))
          d6bb3511f1a85f7d21f48b2ea0247a7a
          # locked
          scm> (stream-to-list (stream-filter both even?) 10)
          6f768e7a9225fc607195b1dfedce8be7
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