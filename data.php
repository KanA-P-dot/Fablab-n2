<?php
// Connexion à la base de données
$servername = "localhost";
$username = "votre_nom_utilisateur";
$password = "votre_mot_de_passe";
$dbname = "nom_de_votre_base_de_données";

$conn = new mysqli($servername, $username, $password, $dbname);

// Vérifier la connexion
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Requête SQL pour récupérer les données
$sql = "SELECT timestamp, avis_prof, avis_cours FROM arduino_data";
$result = $conn->query($sql);

// Afficher les données dans le tableau
if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        echo "<tr>";
        echo "<td>".$row["timestamp"]."</td>";
        echo "<td>".$row["avis_prof"]."</td>";
        echo "<td>".$row["avis_cours"]."</td>";
        echo "</tr>";
    }
} else {
    echo "<tr><td colspan='3'>0 résultats</td></tr>";
}
$conn->close();
?>
