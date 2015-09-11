test = {
  'name': 'interleave-map',
  'points': 1,
  'suites': [
    {
      'cases': [
        {
          'code': r"""
          scm> (define a (cons-stream 1 (cons-stream 3 (cons-stream 5 nil))))
          bd36b05380093e67fd716faf8b2a5fd7
          # locked
          scm> (define b (cons-stream 2 (cons-stream 4 (cons-stream 6 nil))))
          9c8b40007cdee26112d7c67c97af665f
          # locked
          scm> (stream-to-list (interleave-map a b) 10)
          8ed0d6d06d99348b1e6056bb75af814d
          # locked
          """,
          'hidden': False,
          'locked': True
        },
        {
          'code': r"""
          scm> (define a (cons-stream 'a (cons-stream 'b (cons-stream 'c (cons-stream 'd (cons-stream 'e (cons-stream 'f nil)))))))
          bd36b05380093e67fd716faf8b2a5fd7
          # locked
          scm> (define b (cons-stream 1 (cons-stream 2 nil)))
          9c8b40007cdee26112d7c67c97af665f
          # locked
          scm> (stream-to-list (interleave-map a b) 10)
          9ea65ea1d3ff37fe7d88b863652f6c52
          # locked
          """,
          'hidden': False,
          'locked': True
        },
        {
          'code': r"""
          scm> (define ones (cons-stream 1 ones))
          ac617e69bb362320d64986dbad5e6e5e
          # locked
          scm> (define twos (cons-stream 2 twos))
          bec1bafeb4628e1edf583008db853155
          # locked
          scm> (stream-to-list (interleave-map ones twos) 10)
          96e1afc6012fc51179321d4424df06c8
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