test = {
  'name': 'filter',
  'points': 1,
  'suites': [
    {
      'cases': [
        {
          'code': r"""
          scm> (define big-list (int-list 1010 nil))
          big-list
          scm> (filter (lambda (x) (< x 10)) big-list)
          (1 2 3 4 5 6 7 8 9)
          scm> (filter pair? big-list)  ; big-list only has numbers, not pairs
          ()
          scm> (equal? big-list
          ....         (filter (lambda (x) True) big-list))  ; use tail recursion!
          True
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