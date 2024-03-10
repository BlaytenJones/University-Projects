<html>
<body>
<center>
<div style="width:800px;height:1000px;border:10px solid #151973;background-color:rgba(255,255,255,0.84)">
<body background="https://img.freepik.com/free-vector/circuit-board-seamless-pattern_98292-3920.jpg">
<h3>Enter an application in the system to hire the employee associated with it (note: the application will be deleted after this):</h3>

<div>
    <b>Example Hires: </b>
    <table border = "2" bordercolor="330033">
    <thead>
    <tr>
        <th>Employee ID: </th>
	<th>Job ID: </th>
	<th>Location: </th>
	<th>Start Date: </th>
    </tr>
    </thead>
    <tbody>
    <tr>
        <td>0</td>
	<td>1000</td>
	<td>Remote</td>
	<td>4-23-2023</td>
    </tr>
    <tr>
        <td>1</td>
	<td>1001</td>
	<td>Queens, NY</td>
	<td>4-23-2023</td>
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

$Applications = $myDb->query('SELECT * FROM APPLICATIONS NATURAL JOIN JOBS NATURAL JOIN STUDENTS');
?>
<form action="addHiresResults.php" method="post">
	<a>Application: </a>
	<select name="ApplicationChoice">
		<option value="">-------------------- Select --------------------</option>
		<?php foreach ($Applications as $row) { ?>
		<option value="<?php echo $row['STUDENTID'] . ' ' . $row['JOBID'] ?>"><?php echo $row['STUDENTID'] . ': ' . $row['STUDENTNAME'] . '; ' . $row['JOBID'] . ': ' . $row['JOBTITLE']?></option>
		<?php
		}
		?>
	</select>
	<br>
	<br>
	<a>Location: </a>
	<input type="text" name="location">
	<br>
	<br>
	<a>Start Date: </a>
        <input type="date" name="startDate">
	<br>
	<br>
	<input name="submit" type="submit">
</form>

<?php
$myDb->disconnect();
?>

<br><br>
<a href="http://www.csce.uark.edu/~bkj011/project_cpp/hub.html">Go Back to Hub Page!</a>
</body>
</div>
</center>
</body>
</html>
