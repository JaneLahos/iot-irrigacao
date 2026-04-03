<?php

$host = "localhost";
$user = "root";
$pass = "";
$db   = "irrigacao";

$conn = new mysqli($host, $user, $pass, $db);

if ($conn->connect_error) {
    die("Erro conexao: " . $conn->connect_error);
}

$umidade = $_GET['umidade'];
$nivel   = $_GET['nivel'];

$sql = "INSERT INTO leituras (umidade, nivel) VALUES ('$umidade', '$nivel')";

if ($conn->query($sql) === TRUE) {
    echo "OK";
} else {
    echo "Erro: " . $conn->error;
}

$conn->close();

?>
