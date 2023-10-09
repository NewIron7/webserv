<?php
if ($_SERVER["REQUEST_METHOD"] == "GET") {
    if (isset($_GET['name']) && isset($_GET['age'])) {
        $name = $_GET['name'];
        $age = $_GET['age'];

        echo "Name: $name<br>";
        echo "Age: $age<br>";
    } else {
        echo "Both 'name' and 'age' GET variables are required.";
    }
} else {
    echo "This script should be accessed via an HTTP GET request.";
}
?>

