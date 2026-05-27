/*
This is a generative AI model which uses the Makrkov Chain to predict interests and topics in the prompt.
Contains 350 base words stored in flash memory and 50 learnable words stored in the RAM.
This model runs locally on the Arduino UNO R4 with its word dictionary in vocab.h file.

Code designed for the Arduino UNO R4 because of its high fedelity and storage.

Code by Aalok
5 may, 2026
*/
#include "vocab.h"

void setup() {
  Serial.begin(115200);
  while (!Serial); 
  randomSeed(analogRead(A0));
  
  Serial.println("==================================================");
  Serial.println("        Arduino local generative AI model         ");
  Serial.println("==================================================");
  Serial.print(">> Base included words: ");
  Serial.print(BASE_SIZE);
  Serial.println(" base words.");
  Serial.print(">> Learnable SRAM words: ");
  Serial.print(LEARNED_SIZE);
  Serial.println(" slots available.");
  Serial.println();
  Serial.println("Hello human, what can we do? Waiting for input.");
  Serial.println("==================================================");
  Serial.println();
}

int findWordIndex(const char* target_word, bool search_sram) {
  if (search_sram) {
    for (int i = 0; i < current_learned_count; i++) {
      if (strcmp(learned_matrix[i].word, target_word) == 0) return i;
    }
  } else {
    for (int i = 0; i < BASE_SIZE; i++) {
      if (strcmp(base_matrix[i].word, target_word) == 0) return i;
    }
  }
  return -1;
}

void learnConnection(const char* word1, const char* word2) {
  if (strlen(word1) == 0 || strlen(word2) == 0) return;
  if (strlen(word1) > 11 || strlen(word2) > 11) return; 
  
  int sram_index = findWordIndex(word1, true);
  
  if (sram_index != -1) {
    int slot = random(0, 3);
    strcpy(learned_matrix[sram_index].next_words[slot], word2);
    return;
  }
  
  if (sram_index == -1 && current_learned_count < LEARNED_SIZE) {
    int target_slot = current_learned_count;
    strcpy(learned_matrix[target_slot].word, word1);
    
    int flash_index = findWordIndex(word1, false);
    if (flash_index != -1) {
      for (int j = 0; j < 3; j++) {
        strcpy(learned_matrix[target_slot].next_words[j], base_matrix[flash_index].next_words[j]);
      }
    } else {
      for (int j = 0; j < 3; j++) strcpy(learned_matrix[target_slot].next_words[j], word2);
      Serial.print("[LEARNED: '");
      Serial.print(word1);
      Serial.print("' - Slot ");
      Serial.print(current_learned_count + 1);
      Serial.print("/");
      Serial.print(LEARNED_SIZE);
      Serial.println("]");
    }
    
    int slot = random(0, 3);
    strcpy(learned_matrix[target_slot].next_words[slot], word2);
    current_learned_count++;
  }
}

void loop() {
  if (Serial.available() > 0) {
    String userInput = Serial.readStringUntil('\n');
    userInput.trim();
    userInput.toLowerCase();
    
    userInput.replace(".", "");
    userInput.replace("!", "");
    userInput.replace("?", "");
    userInput.replace(",", "");
    
    if (userInput.length() > 0) {
      Serial.print("You: ");
      Serial.println(userInput);
      
      char buffer[128];
      userInput.toCharArray(buffer, 128);
      char* words[20]; 
      int total_words = 0;
      
      char* token = strtok(buffer, " ");
      while (token != NULL && total_words < 20) {
        words[total_words] = token;
        total_words++;
        token = strtok(NULL, " ");
      }
      
      for (int i = 0; i < total_words - 1; i++) {
        learnConnection(words[i], words[i+1]);
      }
      
      Serial.print("AI:  ");
      
      const char* seed_word = "logic"; 
      bool seed_found = false;
      
      for (int w = 0; w < total_words; w++) {
        if (findWordIndex(words[w], true) != -1) {
          seed_word = words[w];
          seed_found = true;
          break;
        }
      }
      
      if (!seed_found) {
        for (int w = 0; w < total_words; w++) {
          if (findWordIndex(words[w], false) != -1) {
            seed_word = words[w];
            seed_found = true;
            break;
          }
        }
      }
      
      const char* current_word = seed_word;
      bool word_found = false;
      
      Serial.print(current_word);
      Serial.print(" ");
      
      // Dynamic Fallback Array (Prevents the AI from repeating the same word if stuck)
      const char* fallbacks[6] = {"interesting", "logic", "friend", "fascinating", "curious", "system"};
      
      for (int word_count = 0; word_count < 9; word_count++) {
        word_found = false;
        const char* chosen_next = "";
        
        int sram_idx = findWordIndex(current_word, true);
        if (sram_idx != -1) {
          chosen_next = learned_matrix[sram_idx].next_words[random(0, 3)];
          word_found = true;
        } 
        else {
          int flash_idx = findWordIndex(current_word, false);
          if (flash_idx != -1) {
            chosen_next = base_matrix[flash_idx].next_words[random(0, 3)];
            word_found = true;
          }
        }
        
        // If there are no known pathways, pick a dynamic AI transition word
        if (!word_found || strlen(chosen_next) == 0) {
          chosen_next = fallbacks[random(0, 6)]; 
        }
        
        current_word = chosen_next;
        Serial.print(current_word);
        Serial.print(" ");
      }
      
      int punct_roll = random(0, 3);
      if (punct_roll == 0) Serial.println(".");
      else if (punct_roll == 1) Serial.println("!");
      else Serial.println("?");
      
      Serial.println();
    }
  }
}