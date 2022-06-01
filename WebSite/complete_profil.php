<?php
    require_once ("init_inc.php");
    
    if($_POST)
    {
        foreach($_POST as $indice => $valeur)
        {
            $_POST[$indice] = htmlEntities(addSlashes($valeur));
        }

        req_execution("INSERT INTO clients (civilite, adresse, ville, cp) VALUES ('$_POST[civilite]', '$_POST[adresse]', '$_POST[ville]', '$_POST[cp]')");

    }

?>

<?php echo $contenu; ?>

<form method="post" action="inscription.php">

    <div>
        <input type="radio" id="Madame" name="Madame" value="Madame">
        <label for="Madame">Madame</label>

        <input type="radio" id="Monsieur" name="Monsieur" value="Monsieur">
        <label for="Monsieur">Monsieur</label>
    </div>

    <label for="nom">Nom</label><br>
    <input type="text" id="nom" name="nom" placeholder= "$_SESSION[nom]" disabled><br><br>
          
    <label for="prenom">Prénom</label><br>
    <input type="text" id="prenom" name="prenom" placeholder=<?php '$_SESSION[prenom]' ?> disabled><br><br>
  
    <label for="email">Email</label><br>
    <input type="email" id="email" name="email" placeholder="$_SESSION[prenom]" disabled><br><br>
    <label for="mdp">Mot de passe</label><br>
    <input type="password" id="mdp" name="mdp" required="required"><br><br>
 
    <input type="submit" name="update" value="Mise à jour">
</form>