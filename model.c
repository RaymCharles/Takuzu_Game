#include "model.h"
#include "game.h"
#include "game_ext.h"
#include "game_tools.h"
#include "game_aux.h"
#include "game_struct.h"
#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
/* **************************************************************** */
#define FONT "Walter.ttf"  // w
#define BACKGROUND "betterback.png"
#define BAMBI "bambi2.png" 
#define PANPAN "panpan3.png"
#define UNDO "fleche gauche.png"
#define REDO "fleche droite.png"
#define RESTART "restartbois.png"
#define EXIT "pancarte.png"
#define PANPAN_COLORE "panpansad.png"
#define BAMBI_COLORE "bambisad.png"
/* **************************************************************** */

/* PUT YOUR VARIABLES HERE */
struct Env_t {
  SDL_Texture* back_ground;
  SDL_Texture* text;
  SDL_Texture* text_over;
  SDL_Texture* bambi2;
  SDL_Texture* panpan3;
  SDL_Texture* undo;
  SDL_Texture* redo;
  SDL_Texture* restart;
  SDL_Texture* exit;
  SDL_Texture* panpan2_colore;
  SDL_Texture* bambi_colore;
  int marge_bottom, marge_top, marge_left, marge_right,marge_title, marge_col;
  game g;
} env;

/* **************************************************************** */

/* PUT YOUR CODE HERE TO INIT TEXTURES, ... */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));
  
  // game's copy in the structure
  if (argc > 2) {
    printf("More than 2 arguments !\n");
    exit (EXIT_FAILURE);
  }
  else if (argc == 2) {
    env->g = game_load(argv[1]);
  }
  else {
    env->g = game_default();
  }

  // get current window size
  int width, height;
  SDL_GetWindowSize(win, &width, &height);

  // print size_line & size_column
  printf("nb_rows = %d\n", game_nb_rows(env->g));
  printf("nb_column = %d\n", game_nb_cols(env->g)); 

  SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
  if (!ren) ERROR("Error: SDL_CreateRenderer (%s)", SDL_GetError());
  // init back_ground texture from PNG image
  env->back_ground = IMG_LoadTexture(ren, BACKGROUND);
  if (!env->back_ground) ERROR("IMG_LoadTexture: %s\n", BACKGROUND);

  // init BAMBI texture from PNG image
  env->bambi2 = IMG_LoadTexture(ren, BAMBI);
  if (!env->bambi2) ERROR("IMG_LoadTexture: %s\n", BAMBI);

  // init PANPAN texture from PNG image
  env->panpan3 = IMG_LoadTexture(ren, PANPAN);
  if (!env->panpan3) ERROR("IMG_LoadTexture: %s\n", PANPAN);

  // init PANPAN COLORED texture from PNG image
  env->panpan2_colore = IMG_LoadTexture(ren, PANPAN_COLORE);
  if (!env->panpan2_colore) ERROR("IMG_LoadTexture: %s\n", PANPAN_COLORE);

  // init BAMBI COLORED texture from PNG image
  env->bambi_colore = IMG_LoadTexture(ren, BAMBI_COLORE);
  if (!env->bambi_colore) ERROR("IMG_LoadTexture: %s\n", BAMBI_COLORE);

  // init EXIT texture from PNG image
  env->exit = IMG_LoadTexture(ren, EXIT);
  if (!env->exit) ERROR("IMG_LoadTexture: %s\n", EXIT);

  // init UNDO texture from PNG image
  env->undo = IMG_LoadTexture(ren, UNDO);
  if (!env->undo) ERROR("IMG_LoadTexture: %s\n", UNDO);

  // init REDO texture from PNG image
  env->redo = IMG_LoadTexture(ren, REDO);
  if (!env->redo) ERROR("IMG_LoadTexture: %s\n", REDO);

  // init RESTART texture from PNG image
  env->restart = IMG_LoadTexture(ren, RESTART);
  if (!env->restart) ERROR("IMG_LoadTexture: %s\n", RESTART);

  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  SDL_Rect rect;

  // get current window size
  int width, height;
  SDL_GetWindowSize(win, &width, &height);
  // ... (SDL_Window *win, ///);  

 // init positions (here because it will change with window size)
  env->marge_col = width / 5;
  env->marge_top = height / 8;
  env->marge_bottom = env->marge_top;
  env->marge_right = (width - env->marge_col) / 10;
  env->marge_left = env->marge_right;
  env->marge_title = height / 10;
 
  
  // render back_ground texture
  SDL_RenderCopy(ren, env->back_ground, NULL, NULL); /* stretch it */

  // init text texture using Arial font
  SDL_Color color = {0, 255, 255, 255};                                     // turquoise in RGBA
  TTF_Font* font = TTF_OpenFont(FONT, width / 30);
  if (!font) ERROR("TTF_OpenFont: %s\n", FONT);
  TTF_SetFontStyle(font, TTF_STYLE_UNDERLINE);                            // TTF_STYLE_ITALIC | TTF_STYLE_NORMAL
  SDL_Surface* surf = TTF_RenderText_Blended(font, "TAKUZU", color);  // blended rendering for ultra nice text
  env->text = SDL_CreateTextureFromSurface(ren, surf);
  SDL_FreeSurface(surf);

  // init TEXT_OVER texture using Arial font
  SDL_Surface* surf2 = TTF_RenderText_Blended(font, "Game is over !", color);
  env->text_over = SDL_CreateTextureFromSurface(ren, surf2);
  SDL_FreeSurface(surf2);
  TTF_CloseFont(font);

  // render text texture
  SDL_QueryTexture(env->text, NULL, NULL, &rect.w, &rect.h);
  rect.x = (width - env->marge_col) / 2 - (rect.w)/2;   // pour centrer en haut le texte "TAKUKU-10a"
  rect.y = (env->marge_title )/ 2;
  SDL_RenderCopy(ren, env->text, NULL, &rect);
  SDL_SetRenderDrawColor(ren, 0, 80, 70, 150);

  // render long straight rectangle
  rect.x = env->marge_left;
  rect.y = env->marge_top + env->marge_title;
  SDL_Rect long_straight_rectangle = {rect.x,rect.y,width - env->marge_col - env->marge_right-env->marge_left,height - env->marge_bottom-env->marge_top-env->marge_title};
  SDL_RenderFillRect(ren, &long_straight_rectangle);

  // rgb color of the long straight rectangle
  /*SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
  for(int i=0; i<2; i++) { // to increase the thickness of the lines, here it's for lines at right
    SDL_RenderDrawLine(ren, width - env->marge_col, 0,width - env->marge_col, height);
  }*/
  
  //Show Grid
  SDL_SetRenderDrawColor(ren, 0, 80, 70, 150);  //Green color
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);  //Green color

  int grid_verti = (height - env->marge_bottom - env->marge_top - env->marge_title) / (game_nb_rows(env->g)); //vertcal space between grid
  int grid_hori = (width - env->marge_right - env->marge_left - env->marge_col) / (game_nb_cols(env->g)); //horizontal space between grid
  // render a grid with lines
  for (int k=0; k<2; k++) { // to double the thickness of the grid
    // Draw horizontal lines
    for (int i = 0; i < game_nb_rows(env->g)+1; i++) {
      SDL_RenderDrawLine(ren,
        env->marge_left -k, 
        env->marge_title + env->marge_top + i* grid_verti -k, 
        width - env->marge_col - env->marge_right -k, 
        env->marge_title + env->marge_top + i* grid_verti -k);
    }
    // Draw vertical lines
    for (int j=0; j < game_nb_cols(env->g)+1; j++) {
      SDL_RenderDrawLine(ren, 
        env->marge_left + j * grid_hori -k, 
        env->marge_title + env->marge_top -k, 
        env->marge_left + j * grid_hori -k, 
        height - env->marge_bottom -k);
    }
  }
  
  // Show game value (S_ONE,...)
  for (int i = 0 ; i < game_nb_rows(env->g) ; i++){
    for (int j = 0 ; j < game_nb_cols(env->g);j++){
      rect.x = env->marge_left + j*grid_hori;   // Pour centrer le BAMBI
      rect.y = env->marge_top + env->marge_title + i*grid_verti;
      rect.w = grid_hori;
      rect.h = grid_verti;
      if (game_get_square(env->g, i , j)==S_ONE ){
        if (game_has_error(env->g,i,j)){
          SDL_RenderCopy(ren, env->bambi_colore, NULL, &rect);
        } 
        else{ 
        SDL_RenderCopy(ren, env->bambi2, NULL, &rect);
        } 
      }
      else if (game_get_square(env->g, i , j) == S_ZERO ){
        if (game_has_error(env->g,i,j)){
          SDL_RenderCopy(ren, env->panpan2_colore, NULL, &rect);
        } 
        else{
          SDL_RenderCopy(ren, env->panpan3, NULL, &rect);
        } 
        
      } 
      else if (game_get_square(env->g, i , j) == S_IMMUTABLE_ONE){
        SDL_RenderCopy(ren, env->bambi2, NULL, &rect);
      } 
      else if (game_get_square(env->g, i , j) == S_IMMUTABLE_ZERO){
        SDL_RenderCopy(ren, env->panpan3, NULL, &rect);
      }
      if (game_is_over(env->g)) {
        // render TEXT_OVER texture
        SDL_QueryTexture(env->text_over, NULL, NULL, &rect.w, &rect.h);
        rect.x = (width - env->marge_col) / 2;
        rect.y = (env->marge_title + env->marge_top ) / 1.5;
        SDL_RenderCopy(ren, env->text_over, NULL, &rect);
        SDL_SetRenderDrawColor(ren, 0, 80, 70, 150);
      }
    } 
  } 

  // Affichage bandeau victoire/défaite (retour à l'ancien comportement)
  for (int i = 0 ; i < game_nb_rows(env->g) ; i++){
    for (int j = 0 ; j < game_nb_cols(env->g);j++){
      rect.x = env->marge_left + j*grid_hori;   // Pour centrer le BAMBI
      rect.y = env->marge_top + env->marge_title + i*grid_verti;
      rect.w = grid_hori;
      rect.h = grid_verti;
      if (game_get_square(env->g, i , j)==S_ONE ){
        if (game_has_error(env->g,i,j)){
          SDL_RenderCopy(ren, env->bambi_colore, NULL, &rect);
        } 
        else{ 
        SDL_RenderCopy(ren, env->bambi2, NULL, &rect);
        } 
      }
      else if (game_get_square(env->g, i , j) == S_ZERO ){
        if (game_has_error(env->g,i,j)){
          SDL_RenderCopy(ren, env->panpan2_colore, NULL, &rect);
        } 
        else{
          SDL_RenderCopy(ren, env->panpan3, NULL, &rect);
        } 
        
      } 
      else if (game_get_square(env->g, i , j) == S_IMMUTABLE_ONE){
        SDL_RenderCopy(ren, env->bambi2, NULL, &rect);
      } 
      else if (game_get_square(env->g, i , j) == S_IMMUTABLE_ZERO){
        SDL_RenderCopy(ren, env->panpan3, NULL, &rect);
      }
      if (game_is_over(env->g)) {
        // render TEXT_OVER texture
        SDL_QueryTexture(env->text_over, NULL, NULL, &rect.w, &rect.h);
        rect.x = (width - env->marge_col) / 2;
        rect.y = (env->marge_title + env->marge_top ) / 1.5;
        SDL_RenderCopy(ren, env->text_over, NULL, &rect);
        SDL_SetRenderDrawColor(ren, 0, 80, 70, 150);
      }
    } 
  } 

  int space = height / 5; //4 correspondant au nombre de boutons
  int button_size =env->marge_col / 1.9;
  rect.w = button_size;
  rect.h = button_size;
  rect.x = width - env->marge_col/2 -rect.h/2 ;   // boutton undo
  rect.y = space;
  SDL_RenderCopy(ren, env->undo, NULL, &rect);

  rect.y = 2* space;
  rect.w = button_size;
  rect.h = button_size;
  rect.x = width - env->marge_col/2-rect.h/2;   // boutton redo
  SDL_RenderCopy(ren, env->redo, NULL, &rect);

  rect.y = 3*space;
  rect.w = button_size;
  rect.h = button_size;
  rect.x = width - env->marge_col/2-rect.h/2;   // boutton restart
  SDL_RenderCopy(ren, env->restart, NULL, &rect);

  rect.y = 4*space;
  rect.w = button_size;
  rect.h = button_size;
  rect.x = width - env->marge_col/2-rect.h/2;   // boutton quit
  SDL_RenderCopy(ren, env->exit, NULL, &rect);
}

/* **************************************************************** */

/* PUT YOUR CODE HERE TO PROCESS EVENTS */

bool process(SDL_Window* win, SDL_Renderer* ren, Env* env, SDL_Event* e) {
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  // generic events
  if (e->type == SDL_QUIT) {
    return true;
  }

  // Android events
  #ifdef __ANDROID__
    else if (e->type == SDL_FINGERDOWN) {
      mouse.x = e->tfinger.x * w; /* tfinger.x, normalized in [0..1] */
      mouse.y = e->tfinger.y * h; /* tfinger.y, normalized in [0..1] */
    }

  // other events
  #else
    else if (e->type == SDL_MOUSEBUTTONDOWN) {
      //SDL_SetRenderDrawColor(ren, 255, 0, 0, 0);
      //SDL_Rect white_button_clicked = {w/2,h/2,(w/10),(w/10)};
      //SDL_RenderFillRect(ren, &white_button_clicked); 
      SDL_Point mouse;
      SDL_GetMouseState(&mouse.x, &mouse.y);

      //init
      int grid_verti = (h - env->marge_bottom - env->marge_top - env->marge_title) / (game_nb_rows(env->g)); //vertcal space between grid
      int grid_hori = (w - env->marge_right - env->marge_left - env->marge_col) / (game_nb_cols(env->g)); //horizontal space between grid
      int space = h / 5; //4 correspondant au nombre de boutons
      int button_size =env->marge_col / 1.9;
    if (mouse.x >= env->marge_left && mouse.x <= w - env->marge_right - env->marge_col && mouse.y >= env->marge_top + env->marge_title && mouse.y <= h - env->marge_bottom)
    {//we clicked in grid
        int j = (mouse.x - env->marge_left)/grid_hori;
        int i = (mouse.y - env->marge_top - env->marge_title)/grid_verti;

        square s = game_get_square(env->g,i,j);
        if (game_is_immutable(env->g, i, j)) {
          printf("Case immuable : modification interdite !\n");
          // Optionnel : jouer un son ou afficher un message graphique
          return false;
        }
        if (s==S_EMPTY){
          game_play_move(env->g, i, j , S_ZERO);
        } 
        else if (s==S_ZERO){
          game_play_move(env->g, i, j , S_ONE);
        } 
        else if (s==S_ONE){
          game_play_move(env->g, i, j , S_EMPTY);
        }     
    } 
    else if (mouse.x>= w - env->marge_col/2 - button_size && mouse.x<= w - env->marge_col/2 + button_size && mouse.y>= space - button_size && mouse.y<= space + button_size) { // pour bouton UNDO
        printf("you clicked on the UNDO button!\n");
        game_undo(env->g);
    }
    else if (mouse.x>= w - env->marge_col/2 - button_size && mouse.x<= w - env->marge_col/2 + button_size && mouse.y>= 2*space - button_size && mouse.y<= 2*space + button_size) { // pour bouton REDO
        printf("you clicked on the REDO button!\n");
        game_redo(env->g);
    }
    else if (mouse.x>= w - env->marge_col/2 - button_size && mouse.x<= w - env->marge_col/2 + button_size && mouse.y>= 3*space - button_size&& mouse.y<= 3*space + button_size) { // pour bouton RESTART
        printf("you clicked on the RESTART button!\n");
        game_restart(env->g);
    }
    else if (mouse.x>= w - env->marge_col/2 - button_size && mouse.x<= w - env->marge_col/2 + button_size && mouse.y>= 4*space - button_size && mouse.y<= 4*space + button_size) { // pour bouton EXIT
        printf("you clicked on the QUIT button!\n");
        return true;
    }    
  }
  #endif
    //SDL_RenderPresent(ren);
    return false; // don't quit

}


/* **************************************************************** */

/* PUT YOUR CODE HERE TO CLEAN MEMORY */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  SDL_DestroyTexture(env->back_ground);
  SDL_DestroyTexture(env->text);
  SDL_DestroyTexture(env->bambi2);
  SDL_DestroyTexture(env->panpan3);
  SDL_DestroyTexture(env->panpan2_colore);
  SDL_DestroyTexture(env->bambi_colore);
  SDL_DestroyTexture(env->restart);
  SDL_DestroyTexture(env->redo);
  SDL_DestroyTexture(env->undo);
  SDL_DestroyTexture(env->exit);
  free(env->g);
  free(env);
}

/* **************************************************************** */