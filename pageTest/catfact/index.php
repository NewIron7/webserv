<!DOCTYPE html>
<html>
<head>
    <title>Cat Facts from Cat Facts API</title>
</head>
<body>
    <h1>Cat Facts</h1>
    <?php
    // PHP code to fetch cat facts from Cat Facts API
    $api_url = 'https://catfact.ninja/fact'; // Endpoint to get cat facts

    $response = file_get_contents($api_url); // Fetch data from the API
    $data = json_decode($response, true); // Decode JSON response

    if ($data && isset($data['fact'])) {
        $cat_fact = $data['fact'];

        // Display fetched cat facts
        echo '<ul>';
        echo '<li>' . $cat_fact . '</li>';
        echo '</ul>';
    } else {
        echo 'Failed to fetch cat facts.';
    }
    ?>
</body>
</html>
