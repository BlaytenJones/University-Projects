import math
import pygame
import time
import abc

from pygame.locals import*
from time import sleep

class Model():
    def __init__(self):
        self.sprites = []
        self.player = Mario(100, 100)
        self.sprites.append(self.player)

    #NOTE: updates the position of all sprites and checks for general collisions
    def update(self):
        check = False
        for i in self.sprites[:]:
            collidee = i
            collidee.update()
            #Removes the sprite if the remove variable is true
            if(collidee.remove):
                self.sprites.remove(i)
            #Checks every sprite compared to a different sprite for collision
            for j in self.sprites[:]:
                collider = j
                #If the sprite colliding with something is not a pipe (as pipes do not move) then check to see that it is not
                #the same sprite as itself. Finally, it determines if the sprite is overlapping with a different sprite.
                if((collider.returnID() != "Pipe") and (i != j) and (collider.collisionDetection(collidee))):
                    check = True
                    #I really tried my best to do something elegant. Have this code instead.
                    if(collider.returnID() == "Mario"):
                        if(collidee.returnID() == "Pipe"):
                            collider.pushOutOfCollision(collidee)
                    elif(collider.returnID() == "Goomba"):
                        if(collidee.returnID() == "Pipe"):
                            collider.pushOutOfCollision(collidee)
                        elif(collidee.returnID() == "Fireball" and collider.flaming == False):
                            #Sets the Goomba on fire if it collides with fireball.
                            collider.flaming = True
                            self.sprites.remove(i)
                            break
                    
        if(not check):
            self.player.isOnPipe = False
            for i in range(len(self.sprites)):
                if(self.sprites[i].returnID() == "Goomba"):
                    self.sprites[i].isOnPipe = False

    #NOTE: sets the sprite's position when given an x and y value
    #It will then use a certain function depending on its ID.
    def addSprite(self, x, y, ID):
        match ID:
            case "Goomba":
                self.sprites.append(Goomba(x, y))
            case "Fireball":
                self.sprites.append(Fireball(self.player.rightFacing, x, y))
            case "Pipe":
                self.sprites.append(Pipe(x, y))





class View():
    def __init__(self, model):
        screen_size = (800,800)
        self.screen = pygame.display.set_mode(screen_size, 32)
        self.model = model
        self.model.rect = pygame.Rect(0, 0, 800, 800)

    def update(self):
        scrollPos = self.model.player.x - 100
        self.screen.fill([7,155,176])
        #NOTE: Iterates over the pipe array and for each element draws it to scene.
        sprites = self.model.sprites
        for i in range(len(self.model.sprites)):
            sprite = sprites[i]
            if(sprite.returnID() == "Mario" or sprite.returnID() == "Goomba"):
                self.screen.blit(sprite.animation[sprite.imageIndex], pygame.Rect(sprite.x - scrollPos, sprite.y, sprite.w, sprite.h))
            else:
                self.screen.blit(sprite.Image, pygame.Rect(sprite.x - scrollPos, sprite.y, sprite.w, sprite.h))
        pygame.draw.rect(self.screen, (205, 155, 0), pygame.Rect(0, 550, 1000, 500))
        pygame.display.flip()





class Controller():
    def __init__(self, model):
        self.model = model
        self.keep_going = True
        self.step = 6
        self.fired = False

    def update(self):
        self.model.player.setPreviousPos()
        for i in range(len(self.model.sprites)):
            if(self.model.sprites[i].returnID() == "Goomba"):
                self.model.sprites[i].setPreviousPos()
        for event in pygame.event.get():
            if event.type == QUIT:
                self.keep_going = False
            elif event.type == KEYUP:
                if (event.key == K_LCTRL or event.key == K_RCTRL):
                    self.fired = False   
            elif event.type == KEYDOWN:
                if (event.key == K_ESCAPE or event.key == K_q):
                    self.keep_going = False
        keys = pygame.key.get_pressed()
        if keys[K_LEFT]:
            self.model.player.rightFacing = False
            self.model.player.x -= self.step
            self.model.player.animate()
        if keys[K_RIGHT]:
            self.model.player.rightFacing = True
            self.model.player.x += self.step
            self.model.player.animate()
        if keys[K_SPACE]:
            if(self.model.player.jumpTimer < 5):
                self.model.player.isOnPipe = False
                self.model.player.vertVelocity -= 6.2
        if ((keys[K_LCTRL] or keys[K_RCTRL]) and not self.fired):
            self.fired = True
            self.model.addSprite(self.model.player.x, self.model.player.y, "Fireball")





# Blayten K. Jones //
#    10/27/2022    //
#Creates a parent class for all visuals//

class Sprite(object):
    def __init__(self, x, y):
        self.x = x; self.y = y
        self.w = 0; self.h = 0
        self.ground = 550
        self.frames = 0
        self.imageIndex = 0
        self.remove = False
        self.lazyLoadImage()

    @abc.abstractmethod
    def update(self):
        pass
    
    #self, graphics, int
    @abc.abstractmethod
    def draw(self, g, scrollPos):
        pass
    
    @abc.abstractmethod
    def lazyLoadImage(self):
        pass

    #returns the "simple" name of the object which represents its ID
    def returnID(self):
        return type(self).__name__

    #self, sprite
    def collisionDetection(self, collider):
        selfRight = self.x + self.w
        colliderRight = collider.x + collider.w
        selfBottom = self.y + self.h
        colliderBottom = collider.y + collider.h
        #checks right collision.
        if(selfRight < collider.x):
            return False
        #checks left collision.
        if(self.x > colliderRight):
            return False
        #checks top collision.
        if(selfBottom < collider.y):
            return False
        #checks bottom collision.
        if(self.y > colliderBottom):
            return False
        
        return True

    def animate(self):
        #Loops between the amount of frames an image has.
        self.imageIndex = (self.imageIndex + 1) % self.frames





# Blayten K. Jones //
#    9/28/2022     //
#Contains the position of each individual pipe//

class Pipe(Sprite):
    
    def __init__(self, x, y):
        self.Image = None
        super().__init__(x, y)
        self.frames = 1
        self.h = self.Image.get_height(); self.w = self.Image.get_width()

    def lazyLoadImage(self):
        #Lazy loads the pipe image; if it is not found, it throws an error.
        try:
            if(self.Image == None):
                self.Image = pygame.image.load("pipe.png")
        except:
            print("There was an error loading the pipe image... EXITING")
            exit(1)

    def existsHere(self, x, y):
        #Determines if a pipe exists at the position at these coordinates.
        return ((x > self.x and x < (self.x + self.w)) and (y > self.y and y < (self.y + self.h)))

    #@Override
    def toString(self):
        return ("Pipe (x,y) = (" + self.x + ", " + self.y + "), width = " + self.w + ", height = " + self.h)





# Blayten K. Jones //
#    10/27/2022    //
#Enemy sprite that moves back and forth//

class Goomba(Sprite):
    def __init__(self, x, y):
        self.px = x; self.py = y
        self.vertVelocity = 0.0
        self.rightFacing = True
        self.flaming = False
        self.animation = []
        self.isOnPipe = False
        self.deleteTimer = 0
        super().__init__(x, y)
        self.h = self.animation[0].get_height(); self.w = self.animation[0].get_width()

    def update(self):
        self.vertVelocity += 1.2
        self.y += self.vertVelocity

        if(self.y > self.ground-self.h):
            self.vertVelocity = 0.0
            self.y = self.ground-self.h; # snap back to the ground

        if(self.isOnPipe):
            self.vertVelocity = 0.0

        #moves the x left or right (depending on which direction it is facing) at a speed of 5. It will stop if it is on fire.
        self.x += ({True: 0, False: 1} [self.flaming]) * ({True: 1, False: -1} [self.rightFacing]) * 5

        #Increments the delete counter if the goomba is currently on fire.
        self.deleteTimer += ({True: 1, False: 0} [self.flaming])

        #Sets the goomba for deletion after 30 cycles of being on fire.
        self.remove = (self.deleteTimer > 30)
        
        self.imageIndex = ({True: 1, False: 0} [self.flaming])
        
        #In the case that it interacts with a pipe diagonally, the gravity will become slower as it is both in the state of being
        #"on the pipe" and falling. This line of code ensures that anytime the character is falling, it is not on the pipe.
        if(self.py != self.y):
            self.isOnPipe = False

    def lazyLoadImage(self):
        #Lazy loads the animation for the goomba; if one of the images is not found, it throws an error.
        if(not self.animation):
            self.frames = 1
            try:
                self.animation.append(pygame.image.load("goomba.png"))
                self.animation.append(pygame.image.load("goomba_fire.png"))
            except:
                print("There was an error loading one of the goomba images... EXITING")
                exit(1)

    def setPreviousPos(self):
        self.px = self.x; self.py = self.y

    def pushOutOfCollision(self, sprite):
        #Top collision.
        #Does not work with the standard height trick...
        if((self.py) <= sprite.y):
            self.y = sprite.y - self.h
            self.isOnPipe = True
            return

        #Left collision.
        if((self.px - self.w) < sprite.x):
            self.rightFacing = False
            #It needs this or else it will teleport through the pipes. This is likely due to the fact that the width is actively changing
            self.px = sprite.x - sprite.w
            self.x = self.px
            
        #Right collision.
        if(self.px > (sprite.x - sprite.w)):
            self.isOnPipe = False
            self.x = sprite.x + sprite.w
            self.rightFacing = True

        #no Bottom collision is needed as goombas don't jump

    def existsHere(self, x, y):
        #Determines if a pipe exists at the position at these coordinates.
        return((x > self.x and x < (self.x + self.w)) and (y > self.y and y < (self.y + self.h)))

    #@Override
    def toString(self):
        return "Goomba (x,y) = (" + self.x + ", " + self.y + "), width = " + self.w + ", height = " + self.h + "; Frame = " + self.imageIndex + "; Flaming = " + self.flaming





# Blayten K. Jones //
#    10/13/2022    //
#Contains the player character's sprites and position//

class Mario(Sprite):
    def __init__(self, x, y):
        self.px = x; self.py = y
        self.vertVelocity = 0.0
        self.rightFacing = True
        self.animation = []
        self.isOnPipe = False
        super().__init__(x, y)
        self.jumpTimer = 0
        self.h = self.animation[0].get_height(); self.w = self.animation[0].get_width()

    def lazyLoadImage(self):
        #Lazy loads the animation for the player; if one of the images is not found, it throws an error.
        if(not self.animation):
            for i in range(5):
                try:
                    self.animation.append(pygame.image.load("mario" + str(i + 1) + ".png"))
                except:
                    print("There was an error loading mario" + str(i + 1) + ".png... EXITING")
                    exit(1)
            self.frames = len(self.animation)

    def update(self):
        self.vertVelocity += 1.2
        self.y += self.vertVelocity
        self.jumpTimer += 1

        if(self.y > self.ground-self.h):
            self.vertVelocity = 0.0
            self.y = self.ground-self.h # snap back to the ground
            self.jumpTimer = 0

        if(self.isOnPipe):
            self.vertVelocity = 0.0
            self.jumpTimer = 0
        
        #In the case that it interacts with a pipe diagonally, the gravity will become slower as it is both in the state of being
        #"on the pipe" and falling. This line of code ensures that anytime the character is falling, it is not on the pipe.
        if(self.py != self.y):
            isOnPipe = False
        

    def pushOutOfCollision(self, sprite):
        #Top collision.
        if((self.py + self.h) <= sprite.y):
            self.isOnPipe = True
            self.y = sprite.y - self.h
            return

        self.isOnPipe = False

        #Bottom collision.
        if(self.py >= (sprite.y + sprite.h)):
            #This part of the collision ensures that if the player "bonks" into the sprite, they will fall back down and have their
            #velocity eaten (this makes the collision feel more realistic).
            self.vertVelocity = 0
            self.jumpTimer = 5
            self.y = sprite.y + sprite.h
            return

        #Left collision.
        if((self.px - self.w) < sprite.x):
            self.x = sprite.x - self.w
        
        
        #Right collision.
        if(self.px > (sprite.x - sprite.w)):
            self.x = sprite.x + sprite.w
        

    def setPreviousPos(self):
        self.px = self.x; self.py = self.y

    #@Override
    def toString(self):
        return "Mario (x,y) = (" + self.x + ", " + self.y + "), width = " + self.w + ", height = " + self.h + "; Frame = " + self.imageIndex + "; Velocity y = " + self.vertVelocity;
    




# Blayten K. Jones //
#    10/27/2022    //
#Projectile sprite that is released by Mario and kills enemies//

class Fireball(Sprite):
    
    def __init__(self, right, x, y):
        self.rightFacing = right
        self.Image = None
        self.existenceTimer = 0
        self.groundX = 0
        self.hitGround = False
        super().__init__(x, y)
        self.w = self.Image.get_width(); self.h = self.Image.get_height()
        self.frames = 1

    def update(self):
        #Counts how many cycles the fireball has existed.
        self.existenceTimer += 1
        if(not self.hitGround):
            self.y += 39
            if(self.y > self.ground - self.h):
                self.y = self.ground - self.h
                #Stores the x at which it hit the ground so that we can offset the sin function
                self.groundX = self.x
                self.hitGround = True
        else:
            #Uses the graph of abs(sin(x)) as a basis for movement. I then stretched both the y and x to give a certain feel.
            #The -12 acts as an offset.
            self.y = self.ground - 165 + (125.0 * abs((math.sin(.025*(self.x - self.groundX)))))
        #Moves the fireball left or right at a speed of 11.
        self.x += ({True: 1, False: -1} [self.rightFacing]) * 11
        #Removes the fireball after 136 cycles of existence (this allows the fireball to keep existing if the player is following it).
        self.remove = self.existenceTimer > 136

    def lazyLoadImage(self):
        try:
            if(self.Image == None):
                self.Image = pygame.image.load("fireball.png")
        except:
            print("There was an error loading the fireball image... EXITING")
            exit(1)

    #@Override
    def toString(self):
        return "Fireball (x,y) = (" + self.x + ", " + self.y + "), width = " + self.w + ", height = " + self.h + "; Frame = " + self.imageIndex;



print("Successfully Loaded!")
print("Use the arrow keys to move. Press space to jump. Press either Ctrl to shoot fireballs. Press Esc or 'Q' to quit.")
pygame.init()
m = Model()
v = View(m)
c = Controller(m)
m.addSprite(474, 281, "Pipe"); m.addSprite(656, 210, "Pipe"); m.addSprite(1098, 298, "Pipe"); m.addSprite(33, 280, "Pipe"); m.addSprite(-189, 402, "Pipe"); m.addSprite(-51, 359, "Pipe"); m.addSprite(750, 450, "Goomba"); m.addSprite(800, 450, "Goomba"); m.addSprite(200, 450, "Goomba")
while c.keep_going:
    c.update()
    m.update()
    v.update()
    sleep(0.04)
print("Goodbye")