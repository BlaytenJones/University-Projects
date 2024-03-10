<html>
<head>
	<style>
		@font-face {
			font-family: 'BigNoodle';
			src: url('bignoodletitling/big_noodle_titling.ttf') format('truetype');
		}
		@font-face {
			font-family: 'Unispace';
			src: url('unispace/unispace rg.otf') format('opentype');
		}
		
		.card {
            		width: 640px;
            		height: 80px;
            		border: 5px solid #ffffff;
            		background-color: rgba(255, 255, 255, 1);
            		border-radius: 20px;	
            		box-sizing: border-box;
            		display: grid;
			flex-direction: column;
            		align-items: center;
			justify-content: center;
			grid-template-columns: 11% 44.5% 44.5%;
            		grid-template-rows: auto;
			gap: 10px;
			grid-column: 2;
			grid-row: 1;
		}
		
		.card:hover {
            		transform: translateY(-5px); /* Elevate the element */
            		box-shadow: 0 5px 15px rgba(0, 0, 0, 1); /* Apply a drop shadow */
        	}

		.cardex {
                        width: 640px;
                        height: 55px;
                        border: 5px solid #ffffff;
                        background-color: rgba(255, 255, 255, 1);
                        border-radius: 20px;
                        box-sizing: border-box;
                        display: grid;
                        flex-direction: column;
                        align-items: center;
                        justify-content: center;
                        box-shadow: 5px 5px 15px 5px rgba(0, 0, 0, 1);
                        grid-template-columns: 11% 44.5% 44.5%;
                        grid-template-rows: auto;
                        gap: 10px;
                        grid-column: 2;
                        grid-row: 1;
                }

		.logo {
            		width: 58%;
			height: 42%;
			position: relative;
			top: -45px;
		}

		.result {
                        padding: 10px;
                        border-radius: 10px;
			border: none;
		}

		.returnedPage {
			width: 750px;
			height: 80px;
			display: grid;
			align-items: center;
			grid-template-columns: 14.285714% 85.714286%;
			gap: 8px;
			color: #000000;
		}

        	.rank {
            		width: 60px;
                        height: 60px;
                        border: 5px solid #ffffff;
                        background-color: rgba(255, 255, 255, 1);
                        border-radius: 60px;
                        box-sizing: border-box;
                        display: grid;
                        flex-direction: column;
                        align-items: center;
                        justify-content: center;
			grid-column: 1;
			grid-row: 1;
			font-size: 45px;
		}

		.rankex {
			width: 40px;
                        height: 40px;
                        border: 5px solid #ffffff;
                        background-color: rgba(255, 255, 255, 1);
                        border-radius: 60px;
                        box-sizing: border-box;
                        display: grid;
                        flex-direction: column;
                        align-items: center;
                        justify-content: center;
                        box-shadow: 5px 5px 15px 5px rgba(0, 0, 0, 0.6);
                        grid-column: 1;
                        grid-row: 1;
                        font-size: 14px;
		}

        	.doc-id {
            		grid-column: 1;
			grid-row: 1;
        	}

        	.doc-name {
            		grid-column: 2;
			grid-row: 1;
        	}

        	.weight {
            		grid-column: 3;
			grid-row: 1;
        	}

		.textbody {
			width: 860px;
			height: 180px;
			border: 7px solid #30428c;
			border-radius: 40px;
			background-color: #30428c;
			margin-top: -40px;
            		position: relative;
			z-index: 1;
			font-family: 'BigNoodle', sans-serif;
			font-size: 30px;
		}

		.radio-group {
			display: flex;
			align-items: center;
    		}
    		.radio-group label {
      			margin-right: 10px;
    		}

		.resultGroup {
			display: flex;
			justify-content: space-around;
			width: 100%;
			align-items: center;
		}

		input[type="radio"]{
			display: inline-block;
			width: auto;
			margin-right: 10px;
			font-size: 15px;
		}

		input[type="text"] {
            		height: 78px;
            		width: 720px;
			font-size: 43px;
			font-family: 'Unispace', monospace;
			box-sizing: border-box;	
			text-align: center;
			border: 7px solid #edf1ff;
			border-radius: 1000px;
			padding: 5px;
			left-margin: 10px;
			background-color: #edf1ff;
		}
		body {
			background-color: #1b1a40;
			color: #ffffff;
                        background-image: linear-gradient(#30428c, #1b1a40);
		}
	</style>
	<script>
        	function resizeText(input) {
            		const maxLength = 22;
            		const currentLength = input.value.length;

            		if (currentLength > maxLength) {
                		const newSize = Math.ceil(43 * (maxLength / currentLength)) + .25*Math.floor(currentLength/79);
                		input.style.fontSize = newSize + 'px';
            		} else {
                		input.style.fontSize = '50px';
            		}
		}
		function resizeTextbody(entries) {
			const textBody = document.querySelector('.textbody');
        		const initialHeight = 216;
			const lineHeight = 148;
			const results = 105;
			const example = 120;
			var newHeight = 0;
			if(entries > 0){
				newHeight = initialHeight + (entries * lineHeight) + results + example;
			}else{
				newHeight = initialHeight + (entries * lineHeight) + results;
			}
			textBody.style.height = newHeight + 'px';
                }
    	</script>
</head>
<body>
	<center>
		<img src="image0.png" class="logo">
		<div class="textbody">
			<body>
				<br><br>
				<form method="post">
					<div class="resultGroup">
						<input type="text" name="query" oninput="resizeText(this)" placeholder="Start typing your query..."><br>
                                                <div class="rank">
							<button type="submit" name="submit">
								<img src="submission-3.png" width="30px" height="30px" border="none">
							</button>
                                                </div>
                                        </div>
					<br>
					<div class="resultGroup">
						Max Number of Results:
                                        	<div class="radio-group">
							<input type="radio" id="option1" name="numResults" value="10" checked>
							<label for="option1">10</label>
                                        	</div>

                                        	<div class="radio-group">
							<input type="radio" id="option2" name="numResults" value="50">
							<label for="option2">50</label>
                                        	</div>

                                        	<div class="radio-group">
							<input type="radio" id="option3" name="numResults" value="100">
							<label for="option3">100</label>
                                        	</div>
					</div>
				</form>
				<?php
				//error_reporting(E_ALL);
				//ini_set('display_errors', 1);
				if (isset($_POST['submit']))
				{
    					// replace ' ' with '\ ' in the strings so they are treated as single command line args
					$query = escapeshellarg($_POST['query']);
					$numResults = escapeshellarg($_POST['numResults']);

    					$command = '/home/bkj011/public_html/searchengine/query ' . $query . ' {*' . $numResults . '}';

    					//echo '<p>' . 'command: ' . $command . '<p>';
    					// remove dangerous characters from command to protect web server
					$command = escapeshellcmd($command);

					$output = shell_exec($command);
					$lines = explode("\n", $output);
					//print_r($output);
					$counter = 0;
					echo 'RESULTS FOR ' . $query;
					foreach ($lines as $line) {
                    				// Check if the line contains the result format
                    				if (preg_match('/^\d+\)\s+(\d+)\s+(\S+)\s+([\d.]+)/', $line, $matches)) {
							if($counter == 0){
								//echo '<div font-size=\'30px\' font-family=\'Unispace\',Monospace> RANK ID WEBSITE WEIGHT </div>';
								echo '<div class="returnedPage">';
                                                        	echo '<div class="rankex">' . RANK . '</div>';
                                                        	echo '<div class="cardex">';
                                                        	echo '<div class="result doc-id">' . ID . '</div>';
                                                        	echo '<div class="result doc-name">' . WEBSITE . '</div>';
                                                        	echo '<div class="result weight">' . WEIGHT . '</div>';
                                                        	echo '</div>';
                                                        	echo '</div>';
                                                        	echo '<br><br>';
							}
							$counter++;
							// Extract information for each result
                        				//$rank = $matches[0];
                        				$docId = $matches[1];
                        				$docName = $matches[2];
                        				$weight = $matches[3];

                        				// Output HTML for each result in a card
							echo '<div class="returnedPage">';
							echo '<div class="rank">' . $counter . '</div>';
							echo '<a href="http://www.csce.uark.edu/~bkj011/searchengine/files/' . $docName . '">';
							echo '<div class="card">';
        						echo '<div class="result doc-id">' . $docId . '</div>';
        						echo '<div class="result doc-name">' . $docName . '</div>';
        						echo '<div class="result weight">' . $weight . '</div>';
							echo '</div>';
							echo '</a>';
							echo '</div>';
							echo '<br><br>';
                    				}
					}

                			// Display the last line outside of a card
					$lastLine = end($lines);
					if($counter == 0){
						echo '<br>UNFORTUNATELY NO RESULTS COULD BE FOUND! TRY A DIFFERENT QUERY';
					}else{
						$lines = explode("\n", $output);
						foreach ($lines as $line){
							if(preg_match('/^RETURNED/', $line)){
								echo $line;
							}
						}
					}
				}
				?>
				<script>
        				resizeTextbody(<?php echo isset($counter) ? $counter : 0; ?>);
    				</script>
			</body>
		</div>
	</center>
</body>
</html>