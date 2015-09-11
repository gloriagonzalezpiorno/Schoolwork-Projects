# A "simple" adventure game.

class Player(object):
    def __init__(self, name, place):
        """Create a player object."""
        self.name = name
        self.place = place
        self.backpack = []

    def look(self):
        self.place.look()

    def go_to(self, direction):
        """Go to direction if it's among the exits of player's current place.
        >>> sather_gate = Place('Sather Gate', 'You are at Sather Gate', [], [])
        >>> gbc = Place('GBC', 'You are at Golden Bear Cafe', [], [])
        >>> sather_gate.add_exits([gbc])
        >>> gbc.add_exits([sather_gate])
        >>> me = Player('player', sather_gate)
        >>> me.go_to('GBC')
        You are at Golden Bear Cafe
        >>> me.place.name
        'GBC'
        >>> me.go_to('GBC')
        Can't go to GBC from GBC.
        Try looking around to see where to go.
        >>> me.go_to('Sather Gate')
        You are at Sather Gate
        """
        "*** YOUR CODE HERE ***"

        self.place = self.place.exit_to(direction)

    def talk_to(self, person):
        """Talk to person if person is at player's current place.
        >>> robert = Character('Robert', 'Have to run for lecture!')
        >>> sather_gate = Place('Sather Gate', 'You are at Sather Gate', [robert], [])
        >>> me = Player('player', sather_gate)
        >>> me.talk_to(robert)
        Person has to be a string.
        >>> me.talk_to('Robert')
        Robert says: Have to run for lecture!
        >>> me.talk_to('Albert')
        Albert is not here.
        """
        if type(person) != str:
            print('Person has to be a string.')
        else:
            if person in self.place.characters.keys():
                print(person + " says: " + self.place.characters[person].message)
            else:
                print(person + ' is not here.')


    def take(self, thing):
        """Take a thing if thing is at player's current place
        >>> hotdog = Thing('Hotdog', 'A hot looking hotdog')
        >>> gbc = Place('GBC', 'You are at Golden Bear Cafe', [], [hotdog])
        >>> me = Player('Player', gbc)
        >>> me.backpack
        []
        >>> me.take(hotdog)
        Thing should be a string.
        >>> me.take('dog')
        dog is not here.
        >>> me.take('Hotdog')
        Player takes the Hotdog
        >>> me.take('Hotdog')
        Hotdog is not here.
        >>> type(me.backpack[0])
        <class '__main__.Thing'>
        """
        if type(thing) != str:
            print('Thing should be a string.')
        else:

            if thing in self.place.things.keys():
                self.backpack.append(thing)
                del self.place.things[thing]
                print ('Player takes the ' + thing)
            else:
                print(thing + ' is not here')

    def check_backpack(self):
        """Print each item with its description and return a list of item names.
        >>> cookie = Thing('Cookie', 'A huge cookie')
        >>> donut = Thing('Donut', 'A huge donut')
        >>> cupcake = Thing('Cupcake', 'A huge cupcake')
        >>> gbc = Place('GBC', 'You are at Golden Bear Cafe', [], [cookie, donut, cupcake])
        >>> me = Player('Player', gbc)
        >>> me.check_backpack()
        In your backpack:
            there is nothing.
        []
        >>> me.take('Cookie')
        Player takes the Cookie
        >>> me.check_backpack()
        In your backpack:
            Cookie - A huge cookie
        ['Cookie']
        >>> me.take('Donut')
        Player takes the Donut
        >>> food = me.check_backpack()
        In your backpack:
            Cookie - A huge cookie
            Donut - A huge donut
        >>> food
        ['Cookie', 'Donut']
        """
        print('In your backpack:')
        if not self.backpack:
            print('    there is nothing.')
        else:
            for item in self.backpack:
                print('   ', item.name, '-', item.description)
        return [item.name for item in self.backpack]


class Character(object):
    def __init__(self, name, message):
        self.name = name
        self.message = message

    def talk(self):
        return self.message


class Thing(object):
    def __init__(self, name, description):
        self.name = name
        self.description = description


class Place(object):
    def __init__(self, name, description, characters, things):
        self.name = name
        self.description = description
        self.characters = {character.name: character for character in characters}
        self.things = {thing.name: thing for thing in things}
        self.exits = {} # {'name': (exit, 'description')}

    def look(self):
        print('You are currently at ' + self.name + '. You take a look around and see:')
        print('Characters:')
        if not self.characters:
            print('    no one in particular')
        else:
            for character in self.characters:
                print('   ', character)
        print('Things:')
        if not self.things:
            print('    nothing in particular')
        else:
            for thing in self.things.values():
                print('   ', thing.name, '-', thing.description)
        self.check_exits()

    def exit_to(self, exit):
        """
        >>> place = gbc.exit_to('Sather Gate')
        You are at Sather Gate
        >>> place is sather_gate
        True

        >>> place = gbc.exit_to('FSM')
        Can't go to FSM from GBC.
        Try looking around to see where to go.
        >>> place is gbc
        True
        """
        if type(exit) != str:
            print('Exit has to be a string.')
            return self
        elif exit in self.exits:
            print(self.exits[exit][1])
            return self.exits[exit][0]
        else:
            print("Can't go to {} from {}.".format(exit, self.name))
            print("Try looking around to see where to go.")
            return self

    def take(self, thing):
        obj = self.things[thing]
        del self.things[thing]
        return obj

    def check_exits(self):
        print('You can exit to:')
        for exit in self.exits:
            print('   ',exit)

    def add_exits(self, places):
        for place in places:
            self.exits[place.name] = (place, place.description)

