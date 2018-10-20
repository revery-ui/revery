
open Reglfw;
open Reglfw.Glfw;

type t = {
    mutable hasLoaded: bool,
    texture: texture,
    mutable width: int,
    mutable height: int,
};

let getTexture = (imagePath: string) => {
    /* TODO: Support url paths? */

    let initialImage = Image.fromColor(255, 0, 0, 255);

    /* Create an initial texture container */
    let texture = glCreateTexture();
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, initialImage);

    let imageLoadPromise = Image.load(imagePath);

    let success = (img) => {
        glBindTexture(GL_TEXTURE_2D, texture);
        /* glTexImage2D(GL_TEXTURE_2D, img); */
        Lwt.return ();
    };

    let _  = Lwt.bind(imageLoadPromise, success);

    texture
};
