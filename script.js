// script.js
document.addEventListener('DOMContentLoaded', function () {
    // Fonction pour charger les avis depuis le serveur
    function loadAvis() {
        fetch('/avis') // L'URL doit correspondre à celle de votre route Flask
            .then(response => response.json())
            .then(data => {
                const tbody = document.querySelector('tbody');
                tbody.innerHTML = ''; // Réinitialiser le contenu du tbody
                data.forEach(avis => {
                    const tr = document.createElement('tr');
                    const td = document.createElement('td');
                    td.textContent = avis;
                    tr.appendChild(td);
                    tbody.appendChild(tr);
                });
            })
            .catch(error => console.error('Erreur de chargement des avis:', error));
    }

    // Charger les avis au chargement de la page
    loadAvis();
});
