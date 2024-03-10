// Blayten K. Jones //
//    9/15/2022     //
//Central unit for the game//

import javax.swing.JFrame;
import java.awt.Toolkit;

public class Game extends JFrame
{
	Controller controller;
	Model model;
	View view;

	public Game()
	{
		//NOTE: Instantiates a player character and a controller connected to the player character.
		model = new Model();
		controller = new Controller(model);
		//NOTE: Instantiates a view which is connected to our controller.
		view = new View(controller, model);
		view.addMouseListener(controller);
		this.addKeyListener(controller);
		//NOTE: Starts editing data of the window itself.
		this.setTitle("A5 - Polymorphism and New Sprites");
		this.setSize(700, 700);
		this.setFocusable(true);
		this.getContentPane().add(view);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setVisible(true);
	}

	public void run(){
		while(true){
			controller.update();
			model.update();
			//NOTE: Indirectly calls the paintComponent() function.
			view.repaint();
			//NOTE: Updates the screen.
			Toolkit.getDefaultToolkit().sync();

			//NOTE: Goes to sleep for 40 milliseconds.
			try{
				Thread.sleep(40);
			}catch(Exception e){
				e.printStackTrace();
				System.exit(1);
			}
		}
	}

	//*****Why String[] args??
	//NOTE: This runs first and establishes the "game" which extends from the frame class and allows us to create a new window.
	public static void main(String[] args)
	{
		Game g = new Game();
		g.run();
	}
}
