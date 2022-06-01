<?php
require_once("inc/init.inc.php");
//--------------------------------- TRAITEMENTS PHP ---------------------------------//
if(isset($_GET['action']) && $_GET['action'] == "deconnexion") 
{
	session_destroy(); 
}
if(internauteEstConnecte()) 
{
	header("location:boutique.php");
}
if($_POST)
{
    $resultat = executeRequete("SELECT * FROM client WHERE email='$_POST[email]'");
    if($resultat->num_rows != 0)
    {
        $client = $resultat->fetch_assoc();
        if($client['mdp'] == $_POST['mdp'])
        {
            if ($client['confirme'] == 1)
            {
                foreach($client as $indice => $element)
                {
                    if($indice != 'mdp')
                    {
                        $_SESSION['client'][$indice] = $element; 
                    }
                }
                header("location:boutique.php");
            }
            else
            {
                $contenu .= "Veullez confirmer votre adresse mail";
                header("location:confirmation.php");
            }
        }
        else
        {
            $contenu .= '<div class="erreur">Erreur de MDP</div>';
        }       
    }
    else
    {
        $contenu .= '<div class="erreur">Aucun compte trouvé avec cette adresse mail
        <a href=".\inscription.php\"><u>Cliquez ici pour vous inscrire</u></a>
        </div>';
    }
}
//--------------------------------- AFFICHAGE HTML ---------------------------------//
?>
<?php require_once("inc/haut.inc.php"); ?>
<?php echo $contenu; ?>
 
<p class="forms-titles">CONNEXION</p>
<form method="post" action="" class="forms">
    <label for="email">Email</label><br />
    <input type="text" id="email" name="email" /><br /> <br />
         
    <label for="mdp">Mot de passe</label><br />
    <input type="password" id="mdp" name="mdp" /><br /><br />
 
     <input type="submit" value="CONNEXION"/>
</form>
 
<?php require_once("inc/bas.inc.php"); ?>