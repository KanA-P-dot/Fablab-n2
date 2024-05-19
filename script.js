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

                    // Colonne Time stamp
                    const timestampTd = document.createElement('td');
                    timestampTd.textContent = avis.timestamp;
                    tr.appendChild(timestampTd);

                    // Colonne Avis Prof
                    const profAvisTd = document.createElement('td');
                    profAvisTd.textContent = avis.prof_avis;
                    tr.appendChild(profAvisTd);

                    // Colonne Avis Cours
                    const coursAvisTd = document.createElement('td');
                    coursAvisTd.textContent = avis.cours_avis;
                    tr.appendChild(coursAvisTd);

                    tbody.appendChild(tr);
                });
            })
            .catch(error => console.error('Erreur de chargement des avis:', error));
    }

    // Charger les avis au chargement de la page
    loadAvis();
});
