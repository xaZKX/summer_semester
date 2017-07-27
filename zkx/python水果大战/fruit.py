import pygame
import sys
import traceback
from pygame.locals import *
from random import *

pygame.init()
pygame.mixer.init()

bg_size=width,height=400,700
screen=pygame.display.set_mode(bg_size)
pygame.display.set_caption("水果大战")
background=pygame.image.load("background.jpg").convert()

BLACK=(0,0,0)
WHITE=(255,255,255)



class MyMario(pygame.sprite.Sprite):
    def __init__(self,bg_size):
        pygame.sprite.Sprite.__init__(self)

        self.image1=pygame.image.load("mary1.png").convert_alpha()
    
        self.active=True
        
        self.rect=self.image1.get_rect()
        self.width,self.height=bg_size[0],bg_size[1]
        self.rect.left,self.rect.top=\
                        (self.width-self.rect.width)//2,\
                        self.height-self.rect.height-60
        self.speed=10
        self.mask=pygame.mask.from_surface(self.image1)


    def moveUp(self):
        if self.rect.top>0:
            self.rect.top-=self.speed
        else:
            self.rect.top=0

    def moveDown(self):
        if self.rect.bottom<self.height-60:
            self.rect.top+=self.speed
        else:
            self.rect.bottom=self.height-60

    def moveLeft(self):
        if self.rect.left>0:
            self.rect.left-=self.speed
        else:
            self.rect.left=0

    def moveRight(self):
        if self.rect.right<self.width:
            self.rect.left+=self.speed
        else:
            self.rect.right=self.width


    def reset(self):
        self.rect.left,self.rect.top=\
                        (self.width-self.rect.width)//2,\
                        self.height-self.rect.height-60
        self.active=True
class Strawberry(pygame.sprite.Sprite):
    def __init__(self,bg_size):
        pygame.sprite.Sprite.__init__(self)

        self.image=pygame.image.load("strawberry.png").convert_alpha()
        self.rect=self.image.get_rect()
        self.width,self.height=bg_size[0],bg_size[1]
        self.speed=3
        self.active=True
        self.rect.left,self.rect.top=\
                            randint(0,self.width-self.rect.width),\
                            randint((-5)*self.height,1)
        self.mask=pygame.mask.from_surface(self.image)

    def move(self):
        if self.rect.top<self.height:
            self.rect.top+=self.speed
        else:
            self.reset()

    def reset(self):
        self.active=True
        self.rect.left,self.rect.top=\
                            randint(0,self.width-self.rect.width),\
                            randint((-5)*self.height,0)


class Orange(pygame.sprite.Sprite):
    energy=2
    def __init__(self,bg_size):
        pygame.sprite.Sprite.__init__(self)
        self.image=pygame.image.load("orange.png").convert_alpha()
        self.rect=self.image.get_rect()
        self.width,self.height=bg_size[0],bg_size[1]
        self.speed=1
        self.active=True
        self.mask=pygame.mask.from_surface(self.image)
        self.rect.left,self.rect.top=\
                            randint(0,self.width-self.rect.width),\
                            randint((-5)*self.height,1)
        self.energy=Orange.energy
        self.hit=False

    def move(self):
        if self.rect.top<self.height:
            self.rect.top+=self.speed
        else:
            self.reset()

    def reset(self):
        self.active=True
        self.energy=Orange.energy
        self.rect.left,self.rect.top=\
                            randint(0,self.width-self.rect.width),\
                            randint((-5)*self.height,0)
        
class Pineapple(pygame.sprite.Sprite):
    energy=4
    def __init__(self,bg_size):
        pygame.sprite.Sprite.__init__(self)

        self.image=pygame.image.load("pineapple.png").convert_alpha()
        self.rect=self.image.get_rect()
        self.width,self.height=bg_size[0],bg_size[1]
        self.speed=1
        self.mask=pygame.mask.from_surface(self.image)
        self.active=True
        self.rect.left,self.rect.top=\
                            randint(0,self.width-self.rect.width),\
                            randint((-5)*self.height,1)
        self.energy=Pineapple.energy
        self.hit=False

    def move(self):
        if self.rect.top<self.height:
            self.rect.top+=self.speed
        else:
            self.reset()

    def reset(self):
        self.active=True
        self.energy=Pineapple.energy
        self.rect.left,self.rect.top=\
                            randint(0,self.width-self.rect.width),\
                            randint((-5)*self.height,0)

class Life_Supply(pygame.sprite.Sprite):
    def __init__(self,bg_size):
        pygame.sprite.Sprite.__init__(self)

        self.image=pygame.image.load("爱心.png").convert_alpha()
        self.rect=self.image.get_rect()
        self.width,self.height=bg_size[0],bg_size[1]
        self.rect.left,self.rect.bottom=\
                                          randint(0,self.width-self.rect.width),-100
        self.speed=5
        self.active=False
        self.mask=pygame.mask.from_surface(self.image)


    def move(self):
        if self.rect.top<self.height:
            self.rect.top+=self.speed
        else:
            self.active=False

    def reset(self):
        self.active=True
        self.rect.left,self.rect.bottom=\
                                          randint(0,self.width-self.rect.width),-100
    
class Bomb_Supply(pygame.sprite.Sprite):
    def __init__(self,bg_size):
        pygame.sprite.Sprite.__init__(self)

        self.image=pygame.image.load("星星.png").convert_alpha()
        self.rect=self.image.get_rect()
        self.width,self.height=bg_size[0],bg_size[1]
        self.rect.left,self.rect.bottom=\
                                          randint(0,self.width-self.rect.width),-100
        self.speed=5
        self.active=False
        self.mask=pygame.mask.from_surface(self.image)


    def move(self):
        if self.rect.top<self.height:
            self.rect.top+=self.speed
        else:
            self.active=False

    def reset(self):
        self.active=True
        self.rect.left,self.rect.bottom=\
                                          randint(0,self.width-self.rect.width),-100
    



class Money_Supply(pygame.sprite.Sprite):
    def __init__(self,bg_size):
        pygame.sprite.Sprite.__init__(self)

        self.image=pygame.image.load("money.png").convert_alpha()
        self.rect=self.image.get_rect()
        self.width,self.height=bg_size[0],bg_size[1]
        self.rect.left,self.rect.top=\
                            randint(0,self.width-self.rect.width),\
                            randint((-5)*self.height,0)

        self.speed=4
        self.active=True
        self.mask=pygame.mask.from_surface(self.image)
   


    def move(self):
        if self.rect.top<self.height:
            self.rect.top+=self.speed
        else:
            self.active=False

    def reset(self):
        self.active=True
        self.rect.left,self.rect.top=\
                            randint(0,self.width-self.rect.width),\
                            randint((-5)*self.height,0)


class Bullet1(pygame.sprite.Sprite):
    def __init__(self,position):
        pygame.sprite.Sprite.__init__(self)

        self.image=pygame.image.load("apple.png").convert_alpha()
        self.rect=self.image.get_rect()
        self.rect.left,self.rect.top=position
        self.speed=12
        self.active=False
        self.mask=pygame.mask.from_surface(self.image)

    def move(self):
        self.rect.top-=self.speed

        if self.rect.top<0:
            self.active=False

    def reset(self,position):
        self.rect.left,self.rect.top=position
        self.active=True

class Bullet2(pygame.sprite.Sprite):
    def __init__(self,position):
        pygame.sprite.Sprite.__init__(self)

        self.image=pygame.image.load("grape.png").convert_alpha()
        self.rect=self.image.get_rect()
        self.rect.left,self.rect.top=position
        self.speed=12
        self.active=False
        self.mask=pygame.mask.from_surface(self.image)

    def move(self):
        self.rect.top-=self.speed

        if self.rect.top<0:
            self.active=False

    def reset(self,position):
        self.rect.left,self.rect.top=position
        self.active=True

pygame.mixer.music.load('back.mp3')
pygame.mixer.music.set_volume(0.5)
pygame.mixer.music.play()
bomb_sound = pygame.mixer.Sound("bomb.wav")
bomb_sound.set_volume(0.8)
money_sound = pygame.mixer.Sound("money.wav")
money_sound.set_volume(0.7)
peng_sound = pygame.mixer.Sound("peng.wav")
peng_sound.set_volume(0.7)
shot_sound = pygame.mixer.Sound("shot.wav")
shot_sound.set_volume(0.2)
buy_sound = pygame.mixer.Sound("buy.wav")
buy_sound.set_volume(0.7)
level_sound = pygame.mixer.Sound("level.wav")
level_sound.set_volume(0.7)

        
def add_strawberry(group1,group2,num):
    for i in range(num):
        e1=Strawberry(bg_size)
        group1.add(e1)
        group2.add(e1)

def add_orange(group1,group2,num):
    for i in range(num):
        e2=Orange(bg_size)
        group1.add(e2)
        group2.add(e2)

def add_pineapple(group1,group2,num):
    for i in range(num):
        e3=Pineapple(bg_size)
        group1.add(e3)
        group2.add(e3)

def add_money(group1,group2,num):
    for i in range(num):
        e4=Money_Supply(bg_size)
        group1.add(e4)
        group2.add(e4)

def inc_speed(target,inc):
    for each in target:
        each.speed+=inc


def main():

    #生成熊猫
    me=MyMario(bg_size)

    enemies=pygame.sprite.Group()
    #生成1号草莓
    first_strawberry=pygame.sprite.Group()
    add_strawberry(first_strawberry,enemies,20)
    #生成2号橘子
    second_orange=pygame.sprite.Group()
    add_orange(second_orange,enemies,4)
    #生成3号菠萝
    third_pineapple=pygame.sprite.Group()
    add_pineapple(third_pineapple,enemies,2)

    
    moneys=pygame.sprite.Group()
    #金币时间
    forth_money=pygame.sprite.Group()
    add_money(forth_money,moneys,6)

    #统计得分
    score=0
    score_font=pygame.font.Font(None,36)

    #统计金钱
    price=0
    price_font=pygame.font.Font(None,36)


    #游戏结束画面
    gameover_font = pygame.font.Font(None, 48)
    gameover0_font = pygame.font.Font(None, 80)

    #设置难度级别
    level=1

    #清屏炸弹
    bomb_image=pygame.image.load("bo.png").convert_alpha()
    bomb_rect=bomb_image.get_rect()
    bomb_font=pygame.font.Font(None,48)
    bomb_num=3


    #每20秒发送一个补给包
    life_supply=Life_Supply(bg_size)
    bomb_supply=Bomb_Supply(bg_size)
    SUPPLY_TIME=USEREVENT
    pygame.time.set_timer(SUPPLY_TIME,20*1000)
    
    

    #生成子弹
    bullet1=[]
    bullet1_index=0
    BULLET1_NUM=6
    for i in range(BULLET1_NUM):
        bullet1.append(Bullet1(me.rect.midtop))

    bullet2=[]
    bullet2_index=0
    BULLET2_NUM=6
    for i in range(BULLET2_NUM):
        bullet2.append(Bullet2(me.rect.midtop))

    clock=pygame.time.Clock()
    

    #用于延迟
    delay=100

    

    #生命数量
    life_image=pygame.image.load("life.png").convert_alpha()
    life_rect=life_image.get_rect()
    life_num=3

    while True:
        for event in pygame.event.get():
            if event.type==QUIT:
                pygame.quit()
                sys.exit()
            elif event.type==MOUSEBUTTONDOWN:
                if event.button==1:
                     pygame.time.set_timer(SUPPLY_TIME,20*1000)
                    
            elif event.type==KEYDOWN:
                if event.key==K_SPACE:
                    if bomb_num>0:
                        bomb_num-=1
                        bomb_sound.play()
                        for each in enemies:
                            if each.rect.bottom>0:
                                each.active=False
            elif event.type==SUPPLY_TIME:
                if choice([True, False]):
                    bomb_supply.reset()
                else:
                    life_supply.reset()


        #根据用户的得分增加难度
        if level==1 and score>50000:
            level=2
            level_sound.play()
            #增加5草莓、3橘子、2菠萝
            add_strawberry(first_strawberry,enemies,5)
            add_orange(second_orange,enemies,3)
            add_pineapple(third_pineapple,enemies,2)
            add_money(forth_money,moneys,5)
            #提升速度
            inc_speed(first_strawberry,1)
        elif level==2 and score>100000:
            level=3
            level_sound.play()
            #增加增加5草莓、3橘子、2菠萝
            add_strawberry(first_strawberry,enemies,5)
            add_orange(second_orange,enemies,3)
            add_pineapple(third_pineapple,enemies,2)
            add_money(forth_money,moneys,5)
            #提升速度
            inc_speed(first_strawberry,1)
            inc_speed(second_orange,1)
        elif level==3 and score>200000:
            level=4
            level_sound.play()
            #增加增加5草莓、3橘子、2菠萝
            add_strawberry(first_strawberry,enemies,5)
            add_orange(second_orange,enemies,3)
            add_pineapple(third_pineapple,enemies,2)
            add_money(forth_money,moneys,5)
            #提升速度
            inc_speed(first_strawberry,1)
            inc_speed(second_orange,1)
      
        
        
        screen.blit(background,(0,0))
        if life_num:
            # 绘制清屏炸弹补给并检测是否获得
            if bomb_supply.active:
                bomb_supply.move()
                screen.blit(bomb_supply.image,bomb_supply.rect)
                if pygame.sprite.collide_mask(bomb_supply,me):
                    money_sound.play()
                    if bomb_num<3:
                        bomb_num+=1
                    bomb_supply.active=False

            # 绘制生命补给并检测是否获得
            if life_supply.active:
                life_supply.move()
                screen.blit(life_supply.image,life_supply.rect)
                if pygame.sprite.collide_mask(life_supply,me):
                    #生命+1
                    money_sound.play()
                    if life_num<3:
                        life_num+=1
                    life_supply.active=False
                    
                    

            # 检测用户的键盘操作
            key_pressed=pygame.key.get_pressed()

            if key_pressed[K_UP]:
                me.moveUp()
            if key_pressed[K_DOWN]:
                me.moveDown()
            if key_pressed[K_LEFT]:
                me.moveLeft()
            if key_pressed[K_RIGHT]:
                me.moveRight()
            if key_pressed[K_s]:                    
                #发射苹果子弹
                if not(delay%10): 
                    bullet1[bullet1_index].reset(me.rect.midtop)
                    bullet1_index=(bullet1_index+1)%BULLET1_NUM
    
                #检测子弹是否击中敌机
                for b in bullet1:
                    if b.active:
                        b.move()
                        screen.blit(b.image,b.rect)
                        enemy_hit=pygame.sprite.spritecollide(b,enemies,False,pygame.sprite.collide_mask)
                        if enemy_hit:
                            b.active=False
                            shot_sound.play()
                            for e in enemy_hit:
                                if e in second_orange or e in third_pineapple:
                                    e.energy-=1
                                    e.hit=True
                                    if e.energy==0:
                                        e.active=False
                                else:
                                    e.active=False
            if key_pressed[K_d]:                    
                #发射葡萄子弹
                if not(delay%10) and price > 20: 
                    bullet2[bullet2_index].reset(me.rect.midtop)
                    bullet2_index=(bullet2_index+1)%BULLET2_NUM
                    price-=50
    
                #检测子弹是否击中敌机
                for b in bullet2:
                    if b.active:
                        b.move()
                        screen.blit(b.image,b.rect)
                        enemy_hit=pygame.sprite.spritecollide(b,enemies,False,pygame.sprite.collide_mask)
                        if enemy_hit:
                            b.active=False
                            shot_sound.play()
                            for e in enemy_hit:
                                if e in second_orange or e in third_pineapple:
                                    e.energy-=2
                                    e.hit=True
                                    if e.energy==0:
                                        e.active=False
                                else:
                                    e.active=False
                                    
            if key_pressed[K_x]:
                buy_sound.play()
                if price>=20000 and bomb_num<3:
                    bomb_num+=1
                    price-=20000
            if key_pressed[K_c]:
                buy_sound.play()
                if price>=30000 and life_num<3:
                    life_num+=1
                    price-=30000


            #绘制金币
            for each in forth_money:
                if each.active:            
                    each.move()
                    screen.blit(each.image,each.rect)
                    if pygame.sprite.collide_mask(each,me):
                        money_sound.play()
                        price+=1000
                        each.active=False
                        each.reset()
             
            #绘制3号菠萝
            for each in third_pineapple:
                if each.active:            
                    each.move()
                    if each.hit:
                        each.hit=False
                    else:    
                        screen.blit(each.image,each.rect)
                else:
                    score+=5000
                    each.reset()
                        
            #绘制2号橘子
            for each in second_orange:
                if each.active:
                    each.move()
                    if each.hit:
                        each.hit=False
                    else:
                        screen.blit(each.image,each.rect)
                else:
                    score+=2000
                    each.reset()
                            

                                
            #绘制1号草莓
            for each in first_strawberry:
                if each.active:
                    each.move()
                    screen.blit(each.image,each.rect)
                else:                 
                    score+=1000
                    each.reset()

            
            #检测超级玛丽是否被撞
            enemies_down=pygame.sprite.spritecollide(me,enemies,False,pygame.sprite.collide_mask)
            if enemies_down :
                peng_sound.play()
                me.active=False
                for e in enemies_down:
                    e.active=False
            

            #绘制超级玛丽
            if me.active:               
                screen.blit(me.image1,me.rect)
            else:
                life_num-=1
                me.reset()

 
            # 绘制清屏炸弹数量
            bomb_text=bomb_font.render("× %d" % bomb_num,True,BLACK)
            text_rect=bomb_text.get_rect()
            screen.blit(bomb_image,(10,height-10-bomb_rect.height))
            screen.blit(bomb_text,(20+bomb_rect.width,height-5-bomb_rect.height))

            #绘制剩余生命数量
            if life_num:
                for i in range(life_num):
                    screen.blit(life_image,\
                                (width-10-(i+1)*life_rect.width,\
                                 height-10-life_rect.height))
            score_text=score_font.render("Score : %s"% str(score),True,WHITE)
            screen.blit(score_text,(10,5))
            money_text=price_font.render("Money : %s"% str(price),True,WHITE)
            screen.blit(money_text,(10,25))
            level_text=price_font.render("Level : %s"% str(level),True,WHITE)
            screen.blit(level_text,(300,5))

            #绘制游戏结束画面
        elif life_num==0:
            pygame.time.set_timer(SUPPLY_TIME,0)
            pygame.mixer.music.stop()
            pygame.mixer.stop()

            #绘制结束界面
            gameover_text1 = gameover_font.render("Your Score:", True, WHITE)
            gameover_text1_rect = gameover_text1.get_rect()
            gameover_text1_rect.left, gameover_text1_rect.top = \
                                 (width - gameover_text1_rect.width) // 2-65, height // 2
            screen.blit(gameover_text1, gameover_text1_rect)
            
            gameover_text2 = gameover_font.render(str(score), True, WHITE)
            gameover_text2_rect = gameover_text2.get_rect()
            gameover_text2_rect.left, gameover_text2_rect.top = \
                                 (width - gameover_text2_rect.width) // 2+100, height // 2
            screen.blit(gameover_text2, gameover_text2_rect)

            gameover_text3 = gameover_font.render("Your money:", True, WHITE)
            gameover_text3_rect = gameover_text3.get_rect()
            gameover_text3_rect.left, gameover_text3_rect.top = \
                                 (width - gameover_text3_rect.width) // 2-65, \
                                 gameover_text1_rect.bottom +50
            screen.blit(gameover_text3, gameover_text3_rect)
 
            gameover_text4 = gameover_font.render(str(price), True, WHITE)
            gameover_text4_rect = gameover_text4.get_rect()
            gameover_text4_rect.left, gameover_text4_rect.top = \
                                 (width - gameover_text4_rect.width) // 2+100, \
                                 gameover_text1_rect.bottom +50
            screen.blit(gameover_text4, gameover_text4_rect)

            gameover_text5 = gameover0_font.render("Game over!", True, WHITE)
            gameover_text5_rect = gameover_text5.get_rect()
            gameover_text5_rect.left, gameover_text5_rect.top = \
                                 (width - gameover_text5_rect.width) // 2, height // 7 
            screen.blit(gameover_text5, gameover_text5_rect)

            
        delay-=1

        if not delay:
            delay=100
            
        pygame.display.flip()

        clock.tick(60)


if __name__=="__main__":
    try:
        main()
    except SystemExit:
        pass
    except:
        traceback.print_exc()
        pygame.quit()
        input()
