<html>
<body>
<center>
<div style="width:800px;height:1000px;border:10px solid #151973;background-color:rgba(255,255,255,0.84)">
<h3>Enter information about the job that you would like to add to the database:</h3>
<body background="https://img.freepik.com/free-vector/circuit-board-seamless-pattern_98292-3920.jpg">
<div>
    <b>Example Jobs: </b>
    <table border = "2" bordercolor="330033">
    <thead>
    <tr>
        <th>ID</th>
	<th>Company</th>
	<th>Job Title</th>
	<th>Salary</th>
	<th>Major</th>
    </tr>
    </thead>
    <tbody>
    <tr>
        <td>1000</td>
	<td>Ronald McDonald Inc.</td>
	<td>Class Clown</td>
	<td>2</td>
	<td>Agribusiness</td>
    </tr>
    <tr>
        <td>1001</td>
	<td>Umbrella Corp.</td>
	<td>Product Tester</td>
	<td>100000</td>
	<td>Computer Science</td>
    </tr>
    </tbody>
    </table>
</div>

<form action="addJobs.php" method="post">
    <br>
    Company: <input type="text" name="jobCompany"><br>
    <br>
    Job Title: <input type="text" name="jobTitle"><br>
    <br>
    Salary: <input type="text" name="jobSalary"><br>
    <br>
    Major: <input type="text" name="jobMajor"><br>
    <br>
    <input name="submit" type="submit" >
</form>
<br><br>

<a style="color:#261c4a" href="http://www.csce.uark.edu/~bkj011/project_cpp/hub.html">Go Back to Hub Page!</a>

<?php
if (isset($_POST['submit']))
{
    // replace ' ' with '\ ' in the strings so they are treated as single command line args
    $jobCompany = escapeshellarg($_POST[jobCompany]);
    $jobTitle = escapeshellarg($_POST[jobTitle]);
    $jobSalary = escapeshellarg($_POST[jobSalary]);
    $jobMajor = escapeshellarg($_POST[jobMajor]);
    
    $command = '/home/bkj011/public_html/project_cpp/addJobs.exe ' . $jobCompany . ' ' . $jobTitle . ' ' . $jobSalary . ' ' . $jobMajor;

    //echo '<p>' . 'command: ' . $command . '<p>';
    // remove dangerous characters from command to protect web server
    $command = escapeshellcmd($command);

    // run addJobs.exe
    system($command);
}
?>
</div>
</center>
</body>
</html>
