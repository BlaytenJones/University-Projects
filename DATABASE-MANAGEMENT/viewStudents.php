<html>
<body>
<center>
<div style="width:800px;height:1000px;border:10px solid #151973;background-color:rgba(255,255,255,0.84)">
<body background="https://img.freepik.com/free-vector/circuit-board-seamless-pattern_98292-3920.jpg">
<h3>See the list of students in the database for a specifc major by choosing it in the dropdown menu below:</h3>

<?php
include("php_db.php");
$myDb = new php_db('bkj011', 'oa8ahYoo', 'bkj011');

//Init database
$myDb->initDatabase();

$Students = $myDb->query('SELECT DISTINCT MAJOR FROM STUDENTS');
?>
<form action="viewStudents.php" method="post">
	<a>Major: </a>
	<select name="MajorChoice">
		<option value="*">Select All</option>
		<?php foreach ($Students as $row) { ?>
		<option value="<?php echo $row['MAJOR'] ?>"><?php echo $row['MAJOR'] ?></option>
		<?php
		}
		?>
	</select>
	<br>
	<br>
	<input name="submit" type="submit">
</form>

<?php
$myDb->disconnect();
?>
<br><br>

<a href="http://www.csce.uark.edu/~bkj011/project_cpp/hub.html">Go Back to Hub Page!</a>

<?php
if (isset($_POST['submit'])) 
{
    // replace ' ' with '\ ' in the strings so they are treated as single command line args
    $major = escapeshellarg($_POST[MajorChoice]);

    $command = '/home/bkj011/public_html/project_cpp/viewStudents.exe ' . $major;

    //echo '<p>' . 'command: ' . $command . '<p>';
    // remove dangerous characters from command to protect web server
    $command = escapeshellcmd($command);
 
    // run addStudents.exe
    system($command);           
}
?>
</body>
</div>
</body>
</center>
</html>
