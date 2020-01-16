#ifndef __BONHOMME_H__
#define __BONHOMME_H__

// les formes
FormeCube *cube = NULL;
FormeQuad *quad = NULL;
FormeSphere *sphere = NULL;
FormeCylindre *cylindre = NULL;
// (partie 1) Vous devez vous servir des quatre fonctions ci-dessous (*sans les modifier*) pour tracer tous les parties des objets.
// affiche un cylindre de rayon 1.0 et de longueur 1.0, dont la base est centrée en (0,0,0)
void afficherCylindre( ) { cylindre->afficher(); }
// affiche une sphère de rayon 1.0, centrée en (0,0,0)
void afficherSphere( ) { sphere->afficher(); }
// affiche un cube d'arête 1.0, centrée en (0,0,0)
void afficherCube( ) { cube->afficher(); }
// affiche un aile d'arête 1
void afficherQuad( ) { quad->afficher(); }

// affiche la position courante du repère (pour débogage)
/*void afficherRepereCourant( int num = 0 )
{
    glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
    FenetreTP::afficherAxes( 1.5, 3.0 );
}*/

// partie 1: le bonhomme
class Bonhomme
{
public:
    Bonhomme()
    {
        initVar();
        // créer le bonhomme graphique
        initialiserGraphique();

        // créer quelques autres formes
        //cube = new FormeCube( 1.0, true );
        //quad = new FormeQuad( 1.0, true );
        //sphere = new FormeSphere( 1.0, 8, 8, true );
        //cylindre = new FormeCylindre( 1.0, 1.0, 1.0, 16, 2, true );
        //afficherCylindre();
    }
    ~Bonhomme()
    {
        conclureGraphique();
        delete cube;
        delete quad;
        delete sphere;
        delete cylindre;
    }

    void initVar() { position = glm::vec3(0.0, 0.0, 2.0); taille = 0.5; angleCorps = angleAile = angleBras = 0.0; }
    void verifierAngles() // vérifier que les angles ne débordent pas les valeurs permises
    {
        if ( angleBras > 90.0 ) angleBras = 90.0; else if ( angleBras < 0.0 ) angleBras = 0.0;
        if ( angleAile > 90.0 ) angleAile = 90.0; else if ( angleAile < 0.0 ) angleAile = 0.0;
    }

    void initialiserGraphique()
    {
        GLint prog = 0; glGetIntegerv( GL_CURRENT_PROGRAM, &prog );
        if ( prog <= 0 )
        {
            std::cerr << "Pas de programme actif!" << std::endl;
            locVertex = locColor = -1;
            return;
        }
        if ( ( locVertex = glGetAttribLocation( prog, "Vertex" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Vertex" << std::endl;
        if ( ( locColor = glGetAttribLocation( prog, "Color" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Color" << std::endl;

        // allouer les objets OpenGL
        glGenVertexArrays( 1, &vao );

        // initialiser le VAO pour la théière
        glBindVertexArray( vao );

        // (partie 2) MODIFICATIONS ICI ...
        // créer le VBO pour les sommets
        //...

        // créer le VBO la connectivité
        //...

        glBindVertexArray(0);
    }

    void conclureGraphique()
    {
        glDeleteBuffers( 1, &vboTheiereSommets );
        glDeleteBuffers( 1, &vboTheiereConnec );
    }

    // (partie 2) Vous modifierez cette fonction pour utiliser les VBOs
    // affiche une théière, dont la base est centrée en (0,0,0)
    void afficherTheiere()
    {
        glBindVertexArray( vao );
        // (partie 2) MODIFICATIONS ICI ...
        //...

        // vous pouvez utiliser temporairement cette fonction pour la première partie du TP, mais vous ferez mieux dans la seconde partie du TP
        glBegin( GL_TRIANGLES );
        for ( unsigned int i = 0 ; i < sizeof(gTheiereConnec)/sizeof(GLuint) ; i++ )
            glVertex3fv( &(gTheiereSommets[3*gTheiereConnec[i]] ) );
        glEnd( );

        glBindVertexArray(0);
    }

    // afficher l'antenne du bonhomme
    // L'antenne  est un cylindre de longueur "taille" et de largeur d'un1/3 de la "taille". Une première partie est fixe au dessus de la tête.
    // Une seconde partie se trouve sur la première partie de l'antenne et tourne deux fois plus vite que angle corps sur elle même selon l'axe Z.
    void afficherAntenne()
    {
        // donner la couleur de l'antenne
        glVertexAttrib3f( locColor, 0.5, 0.5, 1.0 ); // violet; équivalent au glColor() de OpenGL 2.x

        // montrer le repère à la position courante
        //afficherRepereCourant( );

        // afficher la première partie de l'antenne

        //gros cylindre
        matrModel.PushMatrix(); {

            matrModel.Translate(0.0, 0.0, taille*2); // bidon à modifier
            cylindre = new FormeCylindre( taille/3, taille/3, taille, 16, 1, true );
            

            // ==> Avant de tracer, on doit informer la carte graphique des changements faits à la matrice de modélisation
            glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
            afficherCylindre();

            

        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );

        // afficher la seconde partie de l'antenne
        // Procéder comme avec la première partie de l'antenne.
        //...

        //petit cylindre
        matrModel.PushMatrix(); {

            matrModel.Translate(0.0, 0.0, 0.0 + taille*3); // bidon à modifier
            cylindre = new FormeCylindre( taille, taille, taille/3, 16, 1, true );
            

            // ==> Avant de tracer, on doit informer la carte graphique des changements faits à la matrice de modélisation
            glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
            afficherCylindre();

            

        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
    }

    // afficher la tête du bonhomme
    // La tête du bonhomme est une sphère ou theière verte de taille "taille" (ou 0.45 * taille pour la théière)
    // Elle possède des yeux jaunes de taille -> 0.45 * taille situé à l'avant du bonhomme situé sur les cotés de la tête.
    // Les yeux sont à 60 degrés d'écart (30 degrés vers la gauche pour un oeil et 30 degrés vers la droite pour l'autre).
    void afficherTete()
    {
        // montrer le repère à la position courante
        //afficherRepereCourant();

        // donner la couleur de la tête
        glVertexAttrib3f(locColor, 0.0, 1.0, 0.0); // vert; équivalent au glColor() de OpenGL 2.x
        //Pour la tête
        matrModel.PushMatrix(); {

            //Afficher la tête
            matrModel.PushMatrix(); {
                // afficher le bon modèle
                switch (Etat::modele)
                {
                default:
                case 1: // une sphère
                    matrModel.Translate(0.0, 0.0, taille); // (bidon) À MODIFIER
                    sphere = new FormeSphere( taille, 8, 8, true );
                    glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
                    afficherSphere();
                    break;

                case 2: // la théière
                    matrModel.Scale(0.45, 0.45, 0.45);
                    glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
                    afficherTheiere();
                    break;
                }
            }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);

            // donner la couleur des yeux
            glVertexAttrib3f(locColor, 1.0, 1.0, 0.0); // jaune; équivalent au glColor() de OpenGL 2.x

            // afficher les yeux
            //...


            //oeil droit
            matrModel.PushMatrix(); {
                
                
                matrModel.Translate(0.0, 0.0, taille); // (bidon) À MODIFIER
                matrModel.Rotate(30, 0.0 , 0.0, 1.0);
                matrModel.Translate(taille,0.0,0.0);
                sphere = new FormeSphere( 0.45*taille, 8, 8, true );
                glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
                afficherSphere();
                    
            }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);


            //oeil gauche
            matrModel.PushMatrix(); {
                
                
                matrModel.Translate(0.0, 0.0, taille); // (bidon) À MODIFIER
                matrModel.Rotate(-30, 0.0 , 0.0, 1.0);
                matrModel.Translate(taille,0.0,0.0);
                sphere = new FormeSphere( 0.45*taille, 8, 8, true );
                glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
                afficherSphere();
                    
            }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);

        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
    }

    // afficher les deux ailes
    void afficherAiles()
    {
        // donner la couleur des ailes
        glVertexAttrib3f( locColor, 1.0, 0.5, 1.0 ); // magenta clair; équivalent au glColor() de OpenGL 2.x

        // ajouter une ou des transformations afin de tracer des *ailes carrées*, de la même largeur que le corps

        //aile droite
        matrModel.PushMatrix();{
            matrModel.Translate( 0.0, taille, taille ); // (bidon) À MODIFIER
            matrModel.Rotate(angleAile , 1.0 , 0.0 ,0.0 );
            matrModel.Translate(0.0,taille,0.0);
            
            //afficherRepereCourant( ); // débogage
            quad = new FormeQuad( taille*2, true );
            
            glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
            afficherQuad();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );


        //aile gauche
        matrModel.PushMatrix();{
            matrModel.Translate( 0.0, -taille, taille ); // (bidon) À MODIFIER
            matrModel.Rotate(-angleAile , 1.0 , 0.0 ,0.0 );
            matrModel.Translate(0.0,-taille,0.0);
            
            //afficherRepereCourant( ); // débogage
            quad = new FormeQuad( taille*2, true );
            
            glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
            afficherQuad();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
    }

    // afficher les bras et les jambes
    //les bras et les jambes sont des cubes de largeur "largMembre" et longueur "longMembre"
    // Les bras cyan sont situés sur chaque coté de la tête, tournés 80 degrés selon l'axe des X.
    // Ils ont un mouvement de rotation autour de la tête (axe Z) de angleBras du coté vers l'arrière du robot
    // Les jambes marrons sont figés en bas de la tête du robot à 60 degrés d'écart (30 degrés vers la gauche pour une jambe et 30 degrés vers la droite pour l'autre).
    void afficherBrasJambes()
    {
        glVertexAttrib3f( locColor, 0.0, 1.0, 1.0 ); // cyan; équivalent au glColor() de OpenGL 2.x, couleur des bras

        // ajouter une ou des transformations afin de tracer les bras et les jambes de largeur "largMembre" et longueur "longMembre"

        // bras gauche
        matrModel.PushMatrix();{

            matrModel.Translate( 0.0, 0.0, taille ); // (bidon) À MODIFIER
            matrModel.Rotate(angleBras, 0.0 , 0.0 , 1.0 );

            matrModel.Rotate(-10.0, 1.0 , 0.0 , 0.0 );

            matrModel.Translate( 0.0, taille, 0.0 );
            matrModel.Scale(largMembre,longMembre,largMembre);

            matrModel.Translate(0.0,longMembre*1.0/2,0.0);
            

            cube = new FormeCube( 1.0, true );

            //afficherRepereCourant( ); // débogage

            glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
            afficherCube();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );

        //bras droit
        matrModel.PushMatrix();{

            matrModel.Translate( 0.0, 0.0, taille ); // (bidon) À MODIFIER
            matrModel.Rotate(-angleBras, 0.0 , 0.0 , 1.0 );

            matrModel.Rotate(10.0, 1.0 , 0.0 , 0.0 );

            matrModel.Translate( 0.0, -taille, 0.0 );
            matrModel.Scale(largMembre,longMembre,largMembre);

            matrModel.Translate(0.0,-longMembre*1.0/2,0.0);
            

            cube = new FormeCube( 1.0, true );

            //afficherRepereCourant( ); // débogage

            glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
            afficherCube();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );

        //Couleur des jambes
        glVertexAttrib3f(locColor, 0.88, 0.41, 0.0); //marron, couleur des jambes

        // les jambes
        // ...

        //jambe gauche
        matrModel.PushMatrix();{

            matrModel.Translate( 0.0, 0.0, taille ); // (bidon) À MODIFIER
            matrModel.Rotate(-30, 1.0 , 0.0 , 0.0 );

            //matrModel.Rotate(10.0, 1.0 , 0.0 , 0.0 );

            matrModel.Translate( 0.0,0.0, -taille );
            matrModel.Scale(largMembre,largMembre,longMembre);

            matrModel.Translate(0.0,0.0,-longMembre*1.0/2);
            

            cube = new FormeCube( 1.0, true );

            //afficherRepereCourant( ); // débogage

            glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
            afficherCube();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );


        //jambe droite
        matrModel.PushMatrix();{
matrModel.Translate( 0.0, 0.0, taille ); // (bidon) À MODIFIER
            matrModel.Rotate(30, 1.0 , 0.0 , 0.0 );

            //matrModel.Rotate(10.0, 1.0 , 0.0 , 0.0 );

            matrModel.Translate( 0.0,0.0, -taille );
            matrModel.Scale(largMembre,largMembre,longMembre);

            matrModel.Translate(0.0,0.0,-longMembre*1.0/2);
            

            cube = new FormeCube( 1.0, true );

            //afficherRepereCourant( ); // débogage

            glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
            afficherCube();
        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
    }

    void afficher()
    {
        // afficherRepereCourant( ); // débogage
        matrModel.PushMatrix();{ // sauvegarder la tranformation courante

            // ajouter une ou des transformations afin de centrer le haut du corps à la position courante "position[]" et de tourner son corps de l'angle "angleCorps"
            //...

            // afficher le corps
            afficherAntenne();

            // afficher la tête
            afficherTete();

            // afficher les deux ailes
            afficherAiles();

            // afficher les deux jambes
            afficherBrasJambes();

        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel ); // informer ...
    }

    void calculerPhysique()
    {
        if ( Etat::enmouvement )
        {
            static int sens[6] = { +1, +1, +1, +1, +1, +1 };
            glm::vec3 vitesse( 0.03, 0.02, 0.05 );
            // mouvement en X
            if ( position.x-taille <= -0.5*Etat::dimBoite ) sens[0] = +1.0;
            else if ( position.x+taille >= 0.5*Etat::dimBoite ) sens[0] = -1.0;
            position.x += vitesse.x * sens[0];
            // mouvement en Y
            if ( position.y-taille <= -0.5*Etat::dimBoite ) sens[1] = +1.0;
            else if ( position.y+taille >= 0.5*Etat::dimBoite ) sens[1] = -1.0;
            position.y += vitesse.y * sens[1];
            // mouvement en Z
            if ( position.z-taille <= 0.0 ) sens[2] = +1.0;
            else if ( position.z+taille >= Etat::dimBoite ) sens[2] = -1.0;
            position.z += vitesse.z * sens[2];

            // angle des jambes et des ailes
            if ( angleBras <= 0.0 ) sens[3] = +1.0;
            else if ( angleBras >= 90.0 ) sens[3] = -1.0;
            angleBras += 1.0 * sens[3];
            if ( angleAile <= 0.0 ) sens[4] = +1.0;
            else if ( angleAile >= 90.0 ) sens[4] = -1.0;
            angleAile += 2.0 * sens[4];

            // taille du corps
            if ( taille <= 0.25 ) sens[5] = +1.0;
            else if ( taille >= 1.0 ) sens[5] = -1.0;
            taille += 0.005 * sens[5];

            // rotation du corps
            if ( angleCorps > 360.0 ) angleCorps -= 360.0;
            angleCorps += 0.35;

        }
    }

    // partie 2: utilisation de vbo et vao
    GLuint vao = 0;
    GLuint vboTheiereSommets = 0;
    GLuint vboTheiereConnec = 0;
    GLint locVertex = -1;
    GLint locColor = -1;

    glm::vec3 position;       // position courante du bonhomme
    GLfloat taille;           // facteur d'échelle du corps
    GLfloat angleCorps;       // angle de rotation (en degrés) du bonhomme
    GLfloat angleAile;        // angle de rotation (en degrés) des ailes
    GLfloat angleBras;        // angle de rotation (en degrés) des bras
    const GLfloat longMembre = 0.7;  // longueur des membres
    const GLfloat largMembre = 0.1;  // largeur des membres
};

#endif
