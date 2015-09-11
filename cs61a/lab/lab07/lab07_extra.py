from lab07 import *

## Extra Trees Questions ##

# Q5
def add_song(t, song, category):
    """ Returns a new tree with song added to category. Assume the category 
        already exists.

    >>> indie_tunes = tree('indie_tunes', 
    ...                  [tree('indie', 
    ...                    [tree('vance joy',
    ...                       [tree('riptide')])])])
    >>> new_indie = add_song(indie_tunes, 'georgia', 'vance joy')
    >>> print_tree(new_indie)
    indie_tunes
      indie
        vance joy
          riptide
          georgia

    """
    "*** YOUR CODE HERE ***"


# ADT
import inspect
# Tree definition - same Data Abstraction but different implementation from lecture
def tree(entry, subtrees=[]):
    #for subtree in subtrees:
    # assert is_tree(subtree)
    return lambda dispatch: entry if dispatch == 'entry' else list(subtrees)

def entry(tree):
    return tree('entry')

def subtrees(tree):
    return tree('subtrees')

def is_tree(tree):
    try:
        tree_data = inspect.getargspec(tree)
        assert tree_data == inspect.getargspec(lambda dispatch: None)
        return all([is_tree(subtree) for subtree in subtrees(tree)])
    except:
        return False

def is_leaf(tree):
    return not subtrees(tree)

def print_tree(t, indent=0):
    """Print a representation of this tree in which each node is
    indented by two spaces times its depth from the entry.

    >>> print_tree(tree(1))
    1
    >>> print_tree(tree(1, [tree(2)]))
    1
      2
    >>> print_tree(numbers)
    1
      2
      3
        4
        5
      6
        7
    """
    print('  ' * indent + str(entry(t)))
    for subtree in subtrees(t):
        print_tree(subtree, indent + 1)

numbers = tree(1, [tree(2), tree(3, [tree(4), tree(5)]), tree(6, [tree(7)])])