# Q1
def skip_add(n):
    """ Takes a number x and returns x + x-2 + x-4 + x-6 + ... + 0.

    >>> skip_add(5)  # 5 + 3 + 1 + 0
    9
    >>> skip_add(10) # 10 + 8 + 6 + 4 + 2 + 0
    30
    """
    if (n == 1 or n == 0):
        return n
    else:
        return (n + skip_add(n-2))


# Q6
def gcd(a, b):
    """Returns the greatest common divisor of a and b.
    Should be implemented using recursion.

    >>> gcd(34, 19)
    1
    >>> gcd(39, 91)
    13
    >>> gcd(20, 30)
    10
    >>> gcd(40, 40)
    40
    """
    if (a > b):
        if(a % b ==0):
            return b
        else: 
            return gcd(b, a % b)
    if(b > a ):
        if(b%a == 0):
            return a
        else: 
            return gcd(a, b % a)
    if (a == b):
        return a
        


# Q7
def hailstone(n):
    """Print out the hailstone sequence starting at n, and return the
    number of elements in the sequence.

    >>> a = hailstone(10)
    10
    5
    16
    8
    4
    2
    1
    >>> a
    7
    """

    def temp(x, step):
        print(x)
        if(x==1):
            return step
        else:
            if(x%2 == 0 ):
                return temp(x//2, step +1 )
            else:
                return temp(3*x+1, step + 1)
    return temp(n, 1)


# Q8
def fibonacci(n):
    """Return the nth fibonacci number.

    >>> fibonacci(11)
    89
    >>> fibonacci(5)
    5
    >>> fibonacci(0)
    0
    >>> fibonacci(1)
    1
    """
    if (n==1):
        return 1
    if(n==0):
        return 0
    return fibonacci(n-1) + fibonacci(n-2)



# Q9
def paths(m, n):
    """Return the number of paths from one corner of an
    M by N grid to the opposite corner.

    >>> paths(2, 2)
    2
    >>> paths(5, 7)
    210
    >>> paths(117, 1)
    1
    >>> paths(1, 157)
    1
    """
    if( m == 1 or n == 1 ):
        return 1
    else:
        return paths(m-1, n) + paths(m, n-1)
    

