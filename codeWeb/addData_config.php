<?php
 // dang nhap vao database
    $server = "localhost";
    $user = "hunhun"; 
    $pass = "22112001";
    $dbname = "config_mpu";
    
    $conn = mysqli_connect($server,$user,$pass,$dbname);
    
    // Check connection
    if($conn === false){
        die("ERROR: Could not connect. " . mysqli_connect_error());
    }
    $d1 =  $_POST["sample_rate1"];
    $d2 =  $_POST["DLPF1"];
    $d3 =  $_POST["GYRO1"];
    $d4 =  $_POST["accuracy1"];


    
    $sql = "insert into config_value(sample_rate,dlpf,gyro,acc) values('$d1','$d2','$d3','$d4')";
    mysqli_query($conn,$sql);
    mysqli_close($conn);
?>