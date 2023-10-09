<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    if (isset($_POST['name']) && isset($_POST['age'])) {
        $name = $_POST['name'];
        $age = $_POST['age'];

        echo "Name: $name<br>";
        echo "Age: $age<br>";
    } else {
        echo "Both 'name' and 'age' POST variables are required.";
    }
} else {
    echo "This script should be accessed via an HTTP POST request.";
}
?>

