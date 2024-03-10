// Blayten K. Jones //
//    9/15/2022     //
//Sets the destination on the model and takes user input//

import java.awt.event.MouseListener;
import java.awt.event.MouseEvent;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.awt.event.KeyListener;
import java.awt.event.KeyEvent;
import java.util.Iterator;

//implements vs extends? (it states it is different as class -> interface, but what is the difference between these?)
class Controller implements ActionListener, MouseListener, KeyListener
{
	View view;
	Model model;
	boolean keyLeft;
	boolean keyRight;
	boolean keySpace;
	boolean keyControl;
	boolean valid;
	private int step = 6;
	boolean editMode = false;
	boolean pipePlace = true;

	//NOTE: creates a reference to our player by passing it into the controller so that we can directly control the sprite.
	Controller(Model m)
	{
		model = m;
		Json loadObject = Json.load("map.json");
		model.unmarshal(loadObject);
		System.out.println("Game successfully loaded!");
	}

	public void actionPerformed(ActionEvent e)
	{
		
	}

	//NOTE: Allows us to change the current view if needed.
	void setView(View v){
		view = v;
	}

	public void mousePressed(MouseEvent e)
	{
		if(editMode){
			valid = true;
			Iterator<Sprite> it = model.sprites.iterator();
			while(it.hasNext()){
				Sprite object = it.next();
				if((object.returnID() == "Goomba" && ((Goomba)object).existsHere(e.getX() + view.scrollPos, e.getY())) || (object.returnID() == "Pipe" && ((Pipe)object).existsHere(e.getX() + view.scrollPos, e.getY()))){
					it.remove();
					valid = false;
				}
			}
			//Adds a pipe if pipePlace is true (or Goomba if it is false) and the cursor is not on any sprite
			if(valid){
				model.addSprite(e.getX() + view.scrollPos, e.getY(), (pipePlace ? "Pipe" : "Goomba"));
			}
		}
	}

	public void mouseReleased(MouseEvent e){
	}
	public void mouseEntered(MouseEvent e){
	}
	public void mouseExited(MouseEvent e){
	}
	public void mouseClicked(MouseEvent e){
	}


	public void keyPressed(KeyEvent e)
	{
		switch(e.getKeyCode())
		{
			case KeyEvent.VK_RIGHT: keyRight = true; break;
			case KeyEvent.VK_LEFT: keyLeft = true; break;
			case KeyEvent.VK_SPACE: keySpace = true; break;
			case KeyEvent.VK_ESCAPE: System.exit(0);
		}
		char c = Character.toLowerCase(e.getKeyChar());
		if(c == 'q'){
			System.exit(0);
		}else if(c == 's'){
			Json saveObject = model.marshal();
			saveObject.save("map.json");
			System.out.println("Game successfully saved!");
		}else if(c == 'l'){
			Json loadObject = Json.load("map.json");
			model.unmarshal(loadObject);
			System.out.println("Game successfully loaded!");
		}else if(c == 'e'){
			editMode = !editMode;
			System.out.println("Now in " + (editMode ? "edit" : "play") + " mode!");
		}else if(c == 'g'){
			pipePlace = !pipePlace;
			System.out.println("Now in " + (pipePlace ? "pipe" : "goomba") + " placing mode!");
		}
	}

	public void keyReleased(KeyEvent e)
	{
		switch(e.getKeyCode())
		{
			case KeyEvent.VK_RIGHT: keyRight = false; break;
			case KeyEvent.VK_LEFT: keyLeft = false; break;
			case KeyEvent.VK_SPACE: keySpace = false; break;
			case KeyEvent.VK_CONTROL: keyControl = true; break;
		}
	}

	public void keyTyped(KeyEvent e)
	{
	}

	void update(){
		model.player.setPreviousPos();
		for(int i = 0; i < model.sprites.size(); i++){
			if(model.sprites.get(i).returnID() == "Goomba"){
				((Goomba)model.sprites.get(i)).setPreviousPos();
			}
		}
		if(keyRight){
			model.player.rightFacing = true;
			model.player.x += step;
			model.player.animate();
		}
		if(keyLeft){
			model.player.rightFacing = false;
			model.player.x -= step;
			model.player.animate();
		}
		if(keySpace){
			if(model.player.jumpTimer < 5){
				model.player.isOnPipe = false;
				model.player.vertVelocity -= 5.5;
			}
		}
		if(keyControl){
			model.addSprite(model.player.x, model.player.y, "Fireball");
			keyControl = false;
		}
	}

}
