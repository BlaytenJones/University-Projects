<html>
<body>
<center>
<div style="width:800px;height:1000px;border:10px solid #151973;background-color:rgba(255,255,255,0.84)">
<body background="https://img.freepik.com/free-vector/circuit-board-seamless-pattern_98292-3920.jpg">
<h3>Enter information about an application to add to the database:</h3>

<div>
    <b>Example Applications: </b>
    <table border = "2" bordercolor="330033">
    <thead>
    <tr>
        <th>Student ID</th>
	<th>Job ID</th>
    </tr>
    </thead>
    <tbody>
    <tr>
        <td>0</td>
	<td>1000</td>
    </tr>
    <tr>
        <td>1</td>
	<td>1001</td>
    </tr>
    </tbody>
    </table>
</div>
<br>
<?php
include("php_db.php");
$myDb = new php_db('bkj011', 'oa8ahYoo', 'bkj011');

//Init database
$myDb->initDatabase();

$Students = $myDb->query('SELECT * FROM STUDENTS');
$Jobs = $myDb->query('SELECT * FROM JOBS');
?>
<form action="addApplications.php" method="post">
	<a>Student: </a>
	<select name="StudentChoice">
		<option value="">--- Select ---</option>
		<?php foreach ($Students as $row) { ?>
		<option value="<?php echo $row['STUDENTID'] ?>"><?php echo $row['STUDENTID'] . ': ' . $row['STUDENTNAME'] ?></option>
		<?php
		}
		?>
	</select>
	<br>
	<br>
	<a>Jobs: </a>
	<select name="JobChoice">
		<option value="">--- Select ---</option>
		<?php foreach ($Jobs as $row) { ?>
		<option value="<?php echo $row['JOBID'] ?>"><?php echo $row['JOBID'] . ': ' . $row['JOBTITLE'] ?></option>
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
    $studentID = escapeshellarg($_POST[StudentChoice]);
    $jobID = escapeshellarg($_POST[JobChoice]);

    $command = '/home/bkj011/public_html/project_cpp/addApplications.exe ' . $studentID . ' ' . $jobID;

    //echo '<p>' . 'command: ' . $command . '<p>';
    // remove dangerous characters from command to protect web server
    $command = escapeshellcmd($command);
 
    // run addStudents.exe
    system($command);           
}
?>
</body>
</div>
</center>
</body>
</html>
