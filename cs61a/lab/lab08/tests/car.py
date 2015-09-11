test = {
  'name': 'Car',
  'points': 0,
  'suites': [
    {
      'cases': [
        {
          'code': r"""
          >>> from car import *
          >>> roberts_car = Car("Tesla")
          >>> roberts_car.model
          'Tesla'
          """,
          'hidden': False,
          'locked': False
        },
        {
          'code': r"""
          >>> from car import *
          >>> roberts_car = Car("Tesla")
          >>> Car.headlights
          2
          >>> roberts_car.headlights
          2
          >>> Car.headlights = 3
          >>> roberts_car.headlights
          3
          >>> roberts_car.headlights = 2
          >>> Car.headlights
          3
          """,
          'hidden': False,
          'locked': False
        },
        {
          'code': r"""
          >>> from car import *
          >>> roberts_car = Car("Tesla")
          >>> roberts_car.wheels = 2
          >>> roberts_car.wheels
          2
          >>> Car.num_wheels
          4
          >>> roberts_car.drive()
          'Tesla cannot drive!'
          >>> roberts_car.gas
          20
          >>> Car.gas
          30
          """,
          'hidden': False,
          'locked': False
        }
      ],
      'scored': False,
      'type': 'wwpp'
    }
  ]
}