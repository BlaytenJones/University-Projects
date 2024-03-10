<html>
<body>
<center>
<div style="width:800px;height:800px;border:10px solid #151973;background-color:rgba(255,255,255,0.84)">
<body background="https://img.freepik.com/free-vector/circuit-board-seamless-pattern_98292-3920.jpg">
<a href="http://www.csce.uark.edu/~bkj011/project_cpp/addHires.php">Go Back to Input Screen!</a>
<br>
<a href="http://www.csce.uark.edu/~bkj011/project_cpp/hub.html">Go Back to Hub Page!</a>
<h1>RESULTS: </h1>
<?php
if (isset($_POST['submit'])) 
{
    // replace ' ' with '\ ' in the strings so they are treated as single command line args
    $employeeID = escapeshellarg(substr($_POST[ApplicationChoice], 0, strpos($_POST[ApplicationChoice], ' ')));
    $jobID = escapeshellarg(substr($_POST[ApplicationChoice], strpos($_POST[ApplicationChoice], ' ') + 1));
    $location = escapeshellarg($_POST[location]);
    $startDate = escapeshellarg($_POST[startDate]);

    $command = '/home/bkj011/public_html/project_cpp/addHires.exe ' . $employeeID . ' ' . $jobID . ' ' . $location . ' ' . $startDate;

    //echo '<p>' . 'command: ' . $command . '<p>';
    // remove dangerous characters from command to protect web server
    $command = escapeshellcmd($command);
 
    // run addStudents.exe
    system($command);
    $Applications = $myDb->query('SELECT * FROM APPLICATIONS NATURAL JOIN JOBS NATURAL JOIN STUDENTS');    
}
?>
</body>
</div>
</center>
</body>
</html>
