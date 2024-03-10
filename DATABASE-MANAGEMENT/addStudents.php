<html>
<body>
<center>
<div style="width:800px;height:1000px;border:10px solid #151973;background-color:rgba(255,255,255,0.84)">
<h3>Enter information about the student that you would like to add to the database:</h3>
<body background="https://img.freepik.com/free-vector/circuit-board-seamless-pattern_98292-3920.jpg">
<div>
    <b>Example Students: </b>
    <br>
    <table border = "2" bordercolor="330033">
    <thead>
    <tr>
        <th>ID</th>
	<th>Name</th>
	<th>Major</th>
    </tr>
    </thead>
    <tbody>
    <tr>
        <td>0</td>
	<td>Bob Marley</td>
	<td>Agribusiness</td>
    </tr>
    <tr>
        <td>1</td>
	<td>Adam Sandler</td>
	<td>Theater</td>
    </tr>
    </tbody>
    </table>
</div>
<br>
<form action="addStudents.php" method="post">
    Name: <input type="text" name="studentName"><br>
    <br>
    Major: <input type="text" name="studentMajor"><br>
    <br>
    <input name="submit" type="submit" >
</form>
<br><br>

<a style="color:#261c4a" href="http://www.csce.uark.edu/~bkj011/project_cpp/hub.html">Go Back to Hub Page!</a>
<?php
if (isset($_POST['submit']))
{
    // replace ' ' with '\ ' in the strings so they are treated as single command line args
    $studentName = escapeshellarg($_POST[studentName]);
    $studentMajor = escapeshellarg($_POST[studentMajor]);
   
    $command = '/home/bkj011/public_html/project_cpp/addStudents.exe ' . $studentName . ' ' . $studentMajor;

    //echo '<p>' . 'command: ' . $command . '<p>';
    // remove dangerous characters from command to protect web server
    $command = escapeshellcmd($command);

    // run odbc_insert_item.exe
    system($command);
}
?>
</div>
</center>
</body>
</html>
