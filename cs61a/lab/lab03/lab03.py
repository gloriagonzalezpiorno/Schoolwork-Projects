
# Q4
def f_to_c(fahrenheit):
    """Converts Fahrenheit to Celsius

    >>> f_to_c(14)
    -10.0
    >>> f_to_c(68)
    20.0
    >>> f_to_c(-31)
    -35.0
    """
    celcius = (fahrenheit -32) * 5/9 
    return celcius

def c_to_f(celsius):
    """Converts Celsius to Fahrenheit

    >>> c_to_f(0)
    32.0
    >>> c_to_f(5)
    41.0
    >>> c_to_f(-25)
    -13.0
    """
    fahrenheit = celsius* 9/5 + 32 
    return fahrenheit

# Q5
def dispatch_function(option1, f1, option2, f2):
    """Takes in two options and two functions. Returns a function that takes in
    an option and value and calls either f1 or f2 depending on the given option.

    >>> func_d = dispatch_function('c to f', c_to_f, 'f to c', f_to_c)
    >>> func_d('c to f', 0)
    32.0
    >>> func_d('f to c', 68)
    20.0
    >>> func_d('blabl', 2)
    AssertionError
    """
    def h(option, input):
        assert(option==option1 or option == option2)
        if option == option1:
            return f1(input)
        if option == option2:
            return f2(input)
    return h

# Q6
def make_buzzer(n):
    """ Returns a function that prints numbers in a specified
    range except those divisible by n.

    >>> i_hate_fives = make_buzzer(5)
    >>> i_hate_fives(10)
    Buzz!
    1
    2
    3
    4
    Buzz!
    6
    7
    8
    9
    """
    def h(x):
        for i in range(0,x):
            if i % n == 0:
                print('Buzz!')
            else:
                print(i)
    return h


