<!DOCTYPE html>
<html>
<head>
    <title>PHP Request Variables</title>
</head>
<body>
    <h1>PHP Request Variables</h1>

    <?php
    if (isset($_GET['name'])) {
        $name = htmlspecialchars($_GET['name']);
        echo "<p>Name: $name</p>";
    } else {
        echo "<p>Name not found in the GET request.</p>";
    }

	    if (isset($_GET['age'])) {
			        $age = intval($_GET['age']);
					        echo "<p>Age: $age</p>";
					    } else {
							        echo "<p>Age not found in the GET request.</p>";
									    }
	    ?>
</body>
</html>


