import unittest
import requests

class TestWebServer(unittest.TestCase):

    def setUp(self):
        # Set up any necessary configurations or server settings here
        self.server_url = "http://localhost:4245"  # Replace with your server's URL

    def test_server_response(self):
        # Send a GET request to your server and check if it responds with a 200 OK status code
        response = requests.get(self.server_url)
        self.assertEqual(response.status_code, 200)

    def test_server_content(self):
        # Send a GET request to your server and check if it contains expected content
        response = requests.get(self.server_url)
        self.assertIn("Hello World!", response.text)

if __name__ == '__main__':
    unittest.main()

