#include <stdlib.h>
#include <Adafruit_NeoPixel.h>

// node simulates an rgb led
// it is used to store snake as a list of nodes
typedef struct Node{
  int position;
  struct Node* next;

}Node;

// the snake structure that holds its nodes as a linked list
typedef struct Snake{
  int next_move;
  Node* tail;
  Node* head;

}Snake;

// buttons for moving snake
// each should be connected to provided pin number in the arduino
const int UpButton = 6;
const int RightButton = 4;
const int LeftButton = 5;
const int DownButton = 3;

// this variable stores the balls possition
int ball;
Snake* snake;
// the main board of game containing 256 rgb led's
// led is connected to pin 2 of arduino
//it is being handeld only with one pin
Adafruit_NeoPixel led_display = Adafruit_NeoPixel(256, 2, NEO_GRB + NEO_KHZ800);


void initialize_snake(void);
void display_snake(void);
void display_ground(void);
void display_ball(void);
void move_snake(void);
void initialize_ball(void);
void lost(void);

void setup() {
//  initialize base components
  led_display.begin();
  led_display.setBrightness(200);
  led_display.show();
  initialize_snake();
  initialize_ball();

// set input pins to navigating the snake
  pinMode(UpButton, INPUT_PULLUP);
  pinMode(RightButton, INPUT_PULLUP);
  pinMode(LeftButton, INPUT_PULLUP);
  pinMode(DownButton, INPUT_PULLUP);
}


void loop() {

  display_ground();
  display_snake();
  display_ball();
  led_display.show();

// specifiy the next move of snake
// this piece of code is executing 7 times for betterize reading from input
// otherwize the player may push button when delay is called and input is lost
  for(size_t i = 0; i < 7; i++){

    byte up_state=digitalRead(UpButton);
    byte down_state=digitalRead(DownButton);
    byte right_state=digitalRead(RightButton);
    byte left_state=digitalRead(LeftButton);

    if(LOW==up_state){
      snake->next_move=-16;
    }
    else if(LOW==down_state){
      snake->next_move=16;
    }
    else if(LOW==right_state){
      snake->next_move=1;
    }
    else if(LOW==left_state){
      snake->next_move=-1;
    }
    delay(50);
  }

// move the snake based on last move and buttons
  move_snake();

}

void initialize_ball(void){
  
  // set a random possition for bull
  // it wont be part of snake
  int finished=1;

  do{

    finished=1;
    ball = rand() % 256;

    Node* current=snake->tail;

    while(current != NULL){
      if (current->position == ball){
        finished=0;
      }

      current=current->next;
    }

  }while(finished == 0);
}

void move_snake(void){
  // find next position the snake should move
  int current_position=snake->head->position;
  int next_position = current_position + snake->next_move;

// check the move is allowed
  if (next_position > 255 || next_position < 0){
    lost();
    return;
  }
  
  if(((current_position - next_position) == -1) && (current_position % 16 == 15)){
    lost();
    return;
  }

  if(((current_position - next_position) == 1) && (current_position % 16 == 0)){
    lost();
    return;
  }

  Node* current=snake->tail;

  while(current != NULL){
    if (current->position == next_position){
      lost();
      return;
    }

    current=current->next;
  }

  int eated_ball=0;

// checking that ball is eated or not
  if (next_position == ball){
    eated_ball=1;
    initialize_ball();
  }

  Node* new_head=(Node*)calloc(1, sizeof(Node));
  new_head->position=next_position;
  snake->head->next = new_head;
  snake->head=new_head;

  if(0==eated_ball){
    Node* tail=snake->tail;
    snake->tail=tail->next;
    free(tail);
  }

}


void display_ground(void){
  for(size_t i = 0; i < 256; i++){
    led_display.setPixelColor(i,154,205,50);
  }
}

void display_snake(void){

  Node* current=snake->tail;

  while(current != NULL){

    led_display.setPixelColor(current->position, 255, 215, 0);
    current=current->next;
  }
  //display head of snake with different color for better ui:)
  led_display.setPixelColor(snake->head->position, 255, 120, 0);

}

void display_ball(void){
  led_display.setPixelColor(ball, 255, 0, 0);
  
}

void initialize_snake(void){

  Node* node1=(Node*)calloc(1, sizeof(Node));
  Node* node2=(Node*)calloc(1, sizeof(Node));
  Node* node3=(Node*)calloc(1, sizeof(Node));
  
  node1->position=80;
  node2->position=81;
  node3->position=82;

  node1->next=node2;
  node2->next=node3;

  snake=(Snake*)calloc(1, sizeof(Snake));
  snake->next_move=1;
  snake->tail=node1;
  snake->head=node3;

}

void lost(void){
  
  for(int i = 0; i < 4; i++){
    for(size_t i = 0; i < 256; i++){
      led_display.setPixelColor(i, 255, 0, 0);
    }
    led_display.show();
    delay(70);

    for(size_t i = 0; i < 256; i++){
      led_display.setPixelColor(i, 0, 0, 0);
    }
    led_display.show();
    delay(70);
  }

  initialize_snake();
  initialize_ball();
}
