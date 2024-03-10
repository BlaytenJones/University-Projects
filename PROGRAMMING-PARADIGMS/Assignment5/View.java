// Blayten K. Jones //
//    9/15/2022     //
//Implements the player's view//

import javax.swing.JPanel;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.awt.Color;
import javax.imageio.ImageIO;
import java.io.File;

class View extends JPanel
{
	Model model;
	int scrollPos;

	//NOTE: The controller is passed in to connect the button to the actionlistener and to set the current view to this one!
	View(Controller c, Model m)
	{
		scrollPos = 0;
		model = m;
		c.setView(this);
		//NOTE: only fills the memory; it does not actually draw the image to the screen.
	}

	//NOTE: Built in function!
	public void paintComponent(Graphics g){
		scrollPos = model.player.x - 100;
		//NOTE: Sets background color.
		g.setColor(new Color(188, 255, 255)); //Blue
		g.fillRect(0, 0, this.getWidth(), this.getHeight());
		//NOTE: Iterates over the pipe array and for each element draws it to scene.
		for(int i = 0; i < model.sprites.size(); i++){
			model.sprites.get(i).draw(g, scrollPos);
		}
		//Creates the ground sprite.
		g.setColor(Color.ORANGE);
		g.fillRect(0, 550, 2000, 500);
	}

	public static BufferedImage loadImage(String imageName){
		BufferedImage image = null;
		try{
			image = ImageIO.read(new File(imageName));
		}catch(Exception e){

			e.printStackTrace(System.err);
			System.exit(1);
		}
		return image;
	}

	@Override
    public String toString(){
        return "View Scroll Position = " + scrollPos;
    }
}
