<?php
	require_once("inc/init.inc.php");
	 
	    if(isset($_GET['email'], $_GET['key']) AND !empty($_GET['email']) AND !empty($_GET['key'])) {
	    $email = $_GET['email'];
	    $key = htmlspecialchars($_GET['key']);
	    $resultat = executeRequete("SELECT * FROM client WHERE email = '$email' AND confirmatiokey = '$key'");
	    $userexist = $resultat->num_rows;
	    if($userexist == 1) {
	        $client = $resultat->fetch_assoc();
	        if($client['key'] == 0)
            {
                echo "Confirmation de compte!";

	            $confirm = executeRequete ("UPDATE client SET confirme = 1 WHERE email = '$email' AND confirmatiokey = '$key'");
	            header("location:connexion.php"); 
	        }  
            else 
            {
	            echo "Votre compte a déjà été confirmé !";
                header("location:connexion.php"); 
	        } 
	    } 
        else 
        {
	        echo "L'utilisateur n'existe pas !";
            header("location:inscription.php"); 
	    }
	}
?>

<?php require_once("inc/haut.inc.php"); ?>
<?php echo $contenu; ?>
 
<p class="forms-titles">Confirmation d'adresse mail</p>
<form method="GET" action="" class="forms">
    <label for="email">Email</label><br />
    <input type="text" id="email" name="email" /><br /> <br />
         
    <label for="key">Code de confirmation</label><br />
    <input type="key" id="key" name="key" /><br /><br />
 
     <input type="submit" value="CONFIRMER"/>
</form>
 
<?php require_once("inc/bas.inc.php"); ?>