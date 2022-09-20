<?php
 // dang nhap vao database
    $server = "localhost";
    $user = "hunhun"; 
    $pass = "22112001";
    $dbname = "mpu_6050";
    
    $conn = mysqli_connect($server,$user,$pass,$dbname);
 
 // Check connection
    if($conn === false){
        die("ERROR: Could not connect. " . mysqli_connect_error());
    }
    // $tk = $_POST["username"];
    // $mk = $_POST["password"];
    $d1 =  (float) $_POST["bienphp_Ax"];
    $d2 =  (float) $_POST["bienphp_Ay"];
    $d3 =  (float) $_POST["bienphp_Az"];

    $sql = "insert into chart_mpu6050(Ax,Ay,Az) values ($d1,$d2,$d3)";
    mysqli_query($conn,$sql);
    mysqli_close($conn);
?>