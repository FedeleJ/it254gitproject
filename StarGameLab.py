# Preload game canvas
# Import Tk GUI interface module, tkinter and time
from tkinter import *
import time

# Create a Tk window object
tk = Tk()

# Set the window title attribute
tk.title("MB-StarGame")

# Turn off window resizing
tk.resizable(0,0)

# Make the window stay on top
tk.wm_attributes("-topmost",1)

# Create a Canvas object for the window where we can start drawing
canvas = Canvas(tk, width=900, height=700, bd=0, bg="black", highlightthickness=0)

# Resize the borders to the canvas size
canvas.pack()

# Initialize for animation (?)
tk.update()



#Start Game
def startGame():

    #Initial Screen Setup
    canvas.delete("all")

    # Create a Ship class
    class Ship:
        def __init__(self, canvas, myImage, xpos, ypos):
            self.canvas = canvas
            self.id = canvas.create_image(xpos, ypos, anchor=NW, image=myImage)
       
        # Define a methods for flying and firing phaser       
        def fly_Bank(self):
            pos = Falcon.canvas.coords(Falcon.id)
            if pos[0] < 399:
                self.canvas.move(self.id, 8, 1)
            elif pos[0] >= 399:
                self.canvas.move(self.id, 8, -1)
                   
        def fly_E(self):
            self.canvas.move(self.id, 2, 0)

        def firePhaser(self,xpos,ypos,angle):
            lineID=self.canvas.create_line(xpos,ypos,xpos+500,ypos+500,fill="red",width=5)
            if phaserCount >= 5:
                print("fire %s"%angle)
            return lineID

    # Create an Enterprise instance of the Ship class
    EnterpriseImage = PhotoImage(file="C:\School\IT 121\Chapter 12-14/enterprise.gif")
    Enterprise = Ship(canvas, EnterpriseImage, -1000, 10)

    #Create a Falcon instance of the Ship class
    FalconImage = PhotoImage(file="C:\School\IT 121\Chapter 12-14/falcon.gif")
    Falcon = Ship(canvas, FalconImage, 500, 500)

    #Start with phasers off
    global phaserCount
    phaserCount=0
    explosionCount=0
    score=0
    phaserMiss=5
    missCount=3
    
    #Command to fire phasers
    def fireButton():
        global phaserCount
        phaserCount=5
        btn.config(state="disabled")

    #Command to switch phasers on or off
    def switch():
        if btn["state"] == "normal":
            btn["state"] = "disabled"
        else:
            btn["state"] = "normal"

    #Buttons to fire phasers and turn phasers on/off
    btn=Button(tk,text="Fire Phasers",command=fireButton, state="disabled")
    btn.pack()
    btn2=Button(tk,text="Turn phaser on/off",command=switch)
    btn2.pack()

    #Loop to animate
    while True:

        #move the Falcon a little bit
        Falcon.fly_Bank()

        #If the ship left the screen move it back to the start
        pos = Falcon.canvas.coords(Falcon.id)
        if pos[0] > 900:
            Falcon.canvas.move(Falcon.id, -1000, 2)

        #Finish explosions if needed
        if explosionCount == 3:
            ExplosionImage=PhotoImage(file="C:\School\IT 121\Chapter 12-14/explosion-med.gif")
            Explosion=Ship(canvas,ExplosionImage,pos[0]-150,pos[1]-150)
            explosionCount -= 1
        elif explosionCount == 2:
            ExplosionImage=PhotoImage(file="C:\School\IT 121\Chapter 12-14/explosion-lg.gif")
            Explosion=Ship(canvas,ExplosionImage,pos[0]-300,pos[1]-300)
            explosionCount -= 1
        elif explosionCount == 1:
            canvas.delete(Explosion.id)
            explosionCount -= 1

            #Add a score display
            score += 5
        scoreMSG = "Score:"+str(score)
        try:
            canvas.delete(textID)
        except:
            pass
        textID=canvas.create_text(100,600,text=scoreMSG,fill="white",font=("Bauhaus 93",24))

        #move the Enterprise a little bit
        Enterprise.fly_E()
       
        #If the ship left the screen move it back to the start
        pos = Enterprise.canvas.coords(Enterprise.id)
        if pos[0] > 900:
            Enterprise.canvas.move(Enterprise.id, -1900, 0)

        #Delete the last phaser if needed
        try:
            Enterprise.canvas.delete(lineID)
        except:
            pass
        
        if phaserCount > 0:

            #Draw a phaser
            lineID=Enterprise.firePhaser(pos[0]+774,pos[1]+183,"SE")
            phaserCount -= 1    

            #If there is a hit, animate an explosion
            #Determine phaser bank coordinates
            phaserBankXpos=pos[0]+774
            phaserBankYpos=pos[1]+183

            #Determine Falcon coordinates
            pos=Falcon.canvas.coords(Falcon.id)
            FalconCenterXpos=pos[0]+75
            FalconCenterYpos=pos[1]+50

            #Calculate the slope to see if it matches the phaser line of 1:1
            diffXpos=FalconCenterXpos - phaserBankXpos
            diffYpos=FalconCenterYpos - phaserBankYpos

            #Don't devide by 0
            if diffYpos !=0:
    ##              print(diffXpos)
    ##              print(diffYpos)
    ##              print(diffXpos/diffYpos)
                if diffXpos/diffYpos > 0.7 and diffXpos/diffYpos < 1.3:
                    explosionCount=3
                    ExplosionImage=PhotoImage( \
                        file="C:\School\IT 121\Chapter 12-14/explosion-sm.gif")
                    Explosion=Ship(canvas,ExplosionImage,pos[0],pos[1])

                #Display miss count
                if diffXpos/diffYpos <= 0.7 or diffXpos/diffYpos >=1.3:
                    phaserMiss -= 1
                    if phaserMiss == 0:
                        missCount -= 1
                        phaserMiss += 5
        missMSG="MISSES LEFT:"+str(missCount)
        try:
            canvas.delete(missTxt)
        except:
            pass
        missTxt=canvas.create_text(800,600,text=missMSG, \
                                   fill="white",font=("Bauhaus 93",24))
        
        #Game End
        if score == 25:
            canvas.delete("all")
            btn.destroy()
            btn2.destroy()
            winText=canvas.create_text(750,300,anchor=E, \
                                      text=("YOU WIN!"),fill="white",font=("Bauhaus 93",100))
            btn=Button(tk,text="Play Again?", \
                       command=lambda:[btn.destroy(),startGame(),])
            btn.pack()

        if missCount ==0:
            canvas.delete("all")
            btn.destroy()
            btn2.destroy()
            winText=canvas.create_text(750,300,anchor=E, \
                                      text=("YOU LOSE!"),fill="white",font=("Bauhaus 93",100))
            btn=Button(tk,text="Play Again?", \
                       command=lambda:[btn.destroy(),startGame(),])
            btn.pack()

        #refresh the screen
        tk.update_idletasks()
        tk.update()

        #wait a little bit to control animation speed
        time.sleep(0.005)

#Start Screen
startText=canvas.create_text(865,300,anchor=E, \
                             text=("MB-STARGAME"),fill="white",font=("Bauhaus 93",100))
btn=Button(tk,text="Start Game",command=lambda:[btn.destroy(),startGame()])
btn.pack()







