<html>
<body>
<center>
<div style="width:800px;height:1000px;border:10px solid #151973;background-color:rgba(255,255,255,0.84)">
<body background="https://img.freepik.com/free-vector/circuit-board-seamless-pattern_98292-3920.jpg">
<h3>Select from the list below to choose which attribute to search by:</h3>

<?php
include("php_db.php");
$myDb = new php_db('bkj011', 'oa8ahYoo', 'bkj011');

//Init database
$myDb->initDatabase();

$jobMajor = $myDb->query('SELECT DISTINCT DESIREDMAJOR FROM APPLICATIONS NATURAL JOIN JOBS');
$studentMajor = $myDb->query('SELECT DISTINCT MAJOR FROM APPLICATIONS NATURAL JOIN STUDENTS');
$jobID = $myDb->query('SELECT DISTINCT JOBID, JOBTITLE FROM APPLICATIONS NATURAL JOIN JOBS');
$studentID = $myDb->query('SELECT DISTINCT STUDENTID, STUDENTNAME FROM APPLICATIONS NATURAL JOIN STUDENTS');
?>

<form action="viewApplications.php" method="post">
	<a>Attribute: </a>
	<select name="Attribute">
		<option value="*">Select All</option>
		<option value="Jobs">Job ID</option>
		<option value="Desired">Job's Desired Major</option>
		<option value="Majors">Applicant's Major</option>
		<option value="Students">Student ID</option>
	</select>
	<br>
	<br>
	<input name="submitAttr" type="submit">
</form>

<?php
if (isset($_POST['submitAttr']))
{
	$choice = $_POST[Attribute];
        switch($choice){
		case "*":
			//Print all attr
			$choice = escapeshellarg($choice);
			$command = '/home/bkj011/public_html/project_cpp/viewApplications.exe ' . $choice;
			echo '<p>' . 'command: ' . $command . '<p>';
        		$command = escapeshellcmd($command);
        		system($command);
			break;
		case "Jobs":
			//Select by JOB ID
                        ?>
                        <form action="viewApplications.php" method="post">
                                <a>Job ID: </a>
                                <select name="JobIDChoice">
                                        <option value="">--- Select ---</option>
                                        <?php foreach ($jobID as $row) { ?>
                                        <option value="<?php echo $row['JOBID'] ?>"><?php echo $row['JOBID'] . ': ' . $row['JOBTITLE'] ?></option>
                                        <?php
                                        }
                                        ?>
                                </select>
				<br>
				<br>
                                <input name="submitJobID" type="submit">
                        </form>
                        <?php
			break;
		case "Desired":
			//Select by job major
                        ?>
                        <form action="viewApplications.php" method="post">
                                <a>Desired Major: </a>
                                <select name="JobMajorChoice">
                                        <option value="">--- Select ---</option>
                                        <?php foreach ($jobMajor as $row) { ?>
                                        <option value="<?php echo $row['DESIREDMAJOR'] ?>"><?php echo $row['DESIREDMAJOR'] ?></option>
                                        <?php
                                        }
                                        ?>
                                </select>
				<br>
				<br>
                                <input name="submitJobMajor" type="submit">
                        </form>
                        <?php
			break;
		case "Majors":
			//Select by student major
			?>
                        <form action="viewApplications.php" method="post">
				<a>Student Major: </a>
                                <select name="StudentMajorChoice">
                                        <option value="">--- Select ---</option>
                                        <?php foreach ($studentMajor as $row) { ?>
                                        <option value="<?php echo $row['MAJOR'] ?>"><?php echo $row['MAJOR']?></option>
                                        <?php
                                        }
                                        ?>
                                </select>
				<br>
				<br>
                                <input name="submitStudentMajor" type="submit">
                        </form>
                        <?php
			break;
		case "Students":
			//Select by student ID
                        ?>
                        <form action="viewApplications.php" method="post">
                                <a>Student ID: </a>
                                <select name="StudentIDChoice">
                                        <option value="">--- Select ---</option>
                                        <?php foreach ($studentID as $row) { ?>
                                        <option value="<?php echo $row['STUDENTID'] ?>"><?php echo $row['STUDENTID'] . ': ' . $row['STUDENTNAME']?></option>
                                        <?php
                                        }
                                        ?>
                                </select>
				<br>
				<br>
                                <input name="submitStudentID" type="submit">
                        </form>
                        <?php
			break;
		default:
			break;
	}
}

if (isset($_POST['submitJobID']))
{
	$choice = $_POST[JobIDChoice];
	$command = '/home/bkj011/public_html/project_cpp/viewApplications.exe ' . $choice . ' 1';
        //echo '<p>' . 'command: ' . $command . '<p>';
        $command = escapeshellcmd($command);
        system($command);
}
if (isset($_POST['submitJobMajor']))
{
        $choice = $_POST[JobMajorChoice];
        $command = '/home/bkj011/public_html/project_cpp/viewApplications.exe ' . $choice . ' 2';
        //echo '<p>' . 'command: ' . $command . '<p>';
        $command = escapeshellcmd($command);
        system($command);
}
if (isset($_POST['submitStudentMajor']))
{
        $choice = $_POST[StudentMajorChoice];
        $command = '/home/bkj011/public_html/project_cpp/viewApplications.exe ' . $choice . ' 3';
        //echo '<p>' . 'command: ' . $command . '<p>';
        $command = escapeshellcmd($command);
        system($command);
}
if (isset($_POST['submitStudentID']))
{
        $choice = $_POST[StudentIDChoice];
        $command = '/home/bkj011/public_html/project_cpp/viewApplications.exe ' . $choice . ' 4';
        //echo '<p>' . 'command: ' . $command . '<p>';
        $command = escapeshellcmd($command);
        system($command);
}
?>

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
