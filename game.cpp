#include <list>
#include <iostream>
#include <random>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <chrono>
/////////////////////////////////////////////////////////////////////////////////////////
const int   WIDTH = 80; // длина поля
const int   HEIGHT = 20; // высота поля
/////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
using   T_list_ = std::list<T>;
/////////////////////////////////////////////////////////////////////////////////////////
struct T_point
{
    //-----------------------------------------------------------------------------------
    int  x; // координата точки по х
    int  y; // координата точки по у
    //-----------------------------------------------------------------------------------
    T_point
    (
        int x = 0,
        int y = 0
    )
        :
        x(x),
        y(y)
    {}
    //-----------------------------------------------------------------------------------
    bool    operator<   (const T_point& point)   const
    {
        return  std::make_pair(x, y) < std::make_pair(point.x, point.y);
        
    }
    /*
 * Перегженный булеевый оператор делающий так, чтоб make_pair(point.x, point.y) всегда было больше make_pair(x, y)
 * make_pair(x, y) - пара из координат змейки
 * make_pair(point.x, point.y) - пара из координат точки
 */
    //-----------------------------------------------------------------------------------
    T_point operator+=  (const T_point& point)
    {
        x += point.x;
        y += point.y;
        return *this;
        
    }
    /*
        * оператор  изменяет занчение координат змейки
        */
    //-----------------------------------------------------------------------------------
    T_point operator+   (const T_point& point)   const
    {
        T_point res = *this;
        return  res += point;
        
    }
    /*
        * оператор присваевает знаечние коорд.
        */
    //-----------------------------------------------------------------------------------
    bool    operator==  (const T_point& point)   const
    {
        return  x == point.x
            && y == point.y;
       
    }
    /*
       * перегруж. оператор делает так, чтобы x == point.x и y == point.y всегда было истинно
       */
    //-----------------------------------------------------------------------------------
    void    print_position_()                    const
    {
        std::cout
            << x << ' '
            << y << ' '
            << std::endl;
        
    }
    /*
        * функция выводащая координтаы змейки
        */
    //-----------------------------------------------------------------------------------
};
/////////////////////////////////////////////////////////////////////////////////////////
class T_snake
{
    //-----------------------------------------------------------------------------------
    T_list_<T_point>  snake;
    //-----------------------------------------------------------------------------------
public:
    //-----------------------------------------------------------------------------------
    T_snake
    (
        const T_point& start_position
    )
        /*
        * константа начальная позиция змейки
        */
    {
        snake.push_back(start_position);
    }
    /*
    * функция добавляющая в "хвост" змейки 
    */
    //-----------------------------------------------------------------------------------
    T_point             get_head_position_()        const
    {
        return  *snake.begin();
    }
    /* 
    * функция отвещающая за голову змейки
    */
    //-----------------------------------------------------------------------------------
    T_point             get_end_tail_position_()    const
    {
        return *(std::prev(snake.end()));
    }
    /*
    * функция для определения   хвоста змейки
    */
    //-----------------------------------------------------------------------------------
    T_list_<T_point>    get_all_snake_()            const
    {
        return  snake;
    }
    /*
    * функция собирающая воедино змейку
    */
    //-----------------------------------------------------------------------------------
    void                add_tail_snake_()
    {
        snake.push_back(*std::prev(snake.cend()));
    }
    /*
    *  функция увеличивающая хвост змейки
    */
    //-----------------------------------------------------------------------------------
    void                move_new_snake_(T_list_<T_point>&& new_list)
    {
        snake = std::move(new_list);
    }
    /*
    * функция двигающая новую змейку
    */
    //-----------------------------------------------------------------------------------
};
/////////////////////////////////////////////////////////////////////////////////////////
class T_food
{
    //-----------------------------------------------------------------------------------
    T_point position_food;
    //-----------------------------------------------------------------------------------
public:
    //-----------------------------------------------------------------------------------
    T_food
    (
        const T_point& position_food = { 2,2 }
    )
        :
        position_food(position_food)
    {}
    //-----------------------------------------------------------------------------------
    void         generate_new_position_food_()
    {
        static  std::mt19937    gen(std::time(nullptr));
        static  std::uniform_int_distribution<> distance_width(1, WIDTH - 2); // определяется координата еды по x
        static  std::uniform_int_distribution<> distance_height(1, HEIGHT - 2); // определяется координата еды по y
        position_food.x = distance_width(gen); 
        position_food.y = distance_height(gen); 
    }
    /*
    * генерирует новую координату еды
    * position_food.x - задает координата еды по x
    *  position_food.y -  задает координата еды по y
    */
    //-----------------------------------------------------------------------------------
    T_point      get_food_position_()            const
    {
        return  position_food;
    }
    /*
    * задает координату едыd
    */
    //-----------------------------------------------------------------------------------
};
/*
* класс едды
*/
/////////////////////////////////////////////////////////////////////////////////////////
class T_draw_base
{
    //-----------------------------------------------------------------------------------
    COORD        windows_coord_;
    const static HANDLE handle_;
    T_point      coord_;
    //-----------------------------------------------------------------------------------
public:
    //-----------------------------------------------------------------------------------
    T_draw_base
    (
        const T_point& point = { 1,1 }
    )
        :
        coord_(point)
    {}
    /*
    * 
    */
    //-----------------------------------------------------------------------------------
    virtual     ~T_draw_base()
    {}
    /*
    * виртуальная функция нужна для связывания  и разрешения ~T_draw_base()
    */
    //-----------------------------------------------------------------------------------
    void        draw_char_in_coord_(const T_point& point, const char& c = ' ')
    {
        coord_ = point;
        set_windows_coord_();
        SetConsoleCursorPosition(handle_, windows_coord_);
        std::cout.put(c);
        clear_windows_coord_();
    }
    /*
    * функция отвественная за вывод загран. поля
    */
    //-----------------------------------------------------------------------------------
private:
    //-----------------------------------------------------------------------------------
    void        clear_windows_coord_()
    {
        windows_coord_.X = 0;
        windows_coord_.Y = 0;
    }
    /*
    * функция очищает поле от символов
    */
    //-----------------------------------------------------------------------------------
    void        set_windows_coord_()
    {
        windows_coord_.X = coord_.x;
        windows_coord_.Y = coord_.y;
    }
    /*
    * функция устанавливает символ в каждой ячейке
    */
    //-----------------------------------------------------------------------------------
};
/////////////////////////////////////////////////////////////////////////////////////////
const HANDLE T_draw_base::handle_ = GetStdHandle(STD_OUTPUT_HANDLE);
/*
    * константа   отвественен за вывод консоли
    */
/////////////////////////////////////////////////////////////////////////////////////////
class T_draw_field : public T_draw_base
{
public:
    //-----------------------------------------------------------------------------------
    T_draw_field()
    {}
    //-----------------------------------------------------------------------------------
    void        draw_field_()
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            for (int y = 0; y < HEIGHT; ++y)
            {
                if
                    (
                        x == 0
                        || x == WIDTH - 1
                        || y == 0
                        || y == HEIGHT - 1
                        )
                {
                    draw_char_in_coord_({ x,y }, '#');
                } // if
            } // for
        } // for
    }
    /*
    * функция для призыва поля
    */
    //-----------------------------------------------------------------------------------
};
/////////////////////////////////////////////////////////////////////////////////////////
class T_draw_snake : public T_draw_base
{
    //-----------------------------------------------------------------------------------
public:
    //-----------------------------------------------------------------------------------
    T_draw_snake()
    {}
    //-----------------------------------------------------------------------------------
    void        draw_snake_(const T_snake& snake)
    {
        auto snake_tail = snake.get_all_snake_();
        for (auto i = snake_tail.cbegin(); i != snake_tail.cend(); ++i)
        {
            if (*i == snake.get_head_position_())
            {
                draw_char_in_coord_(*i, 'O');
            }
            else
            {
                draw_char_in_coord_(*i, 'o');
            }
        }
    }
    /*
    * функция выводящая змейку
    *  snake_tail - хвост змейки, выводящийся через цикл рисующий i кол-во 0 в хвост змейки
    */
    //-----------------------------------------------------------------------------------
};
/////////////////////////////////////////////////////////////////////////////////////////
class T_snake_game
{
    //-----------------------------------------------------------------------------------
    T_snake         snake_;
    T_draw_base     drawer_base_;
    T_draw_field    drawer_field_;
    T_draw_snake    drawer_snake_;
    T_food          food_;
    int             size_snake_;
    enum
    {
        STOP,

        UP,
        DOWN,
        LEFT,
        RIGHT
    } direction;
    /*
    * команды для управления змейкой
    */
    //-----------------------------------------------------------------------------------
public:
    //-----------------------------------------------------------------------------------
    T_snake_game
    (
    )
        :
        snake_({ 40,10 }),
        size_snake_(0)
    {}/*
    * размер змеи
    */
    //-----------------------------------------------------------------------------------
private:
    //-----------------------------------------------------------------------------------
    void        initial_varialbes_and_draw_()
    {
        drawer_field_.draw_field_();
        food_.generate_new_position_food_();
        drawer_base_.draw_char_in_coord_(food_.get_food_position_(), '@');
        drawer_snake_.draw_snake_(snake_);
        /*
    * войд функция, ответсвенная за отрисовку всех элементов
    * drawer_base_.draw_char_in_coord_(food_.get_food_position_(), '@'); - вывод еду в виде @
    * drawer_field_.draw_field_(); - рисует поле
    * food_.generate_new_position_food_() - генерирует поле 
    * drawer_snake_.draw_snake_(snake_); - рисует саму змейку
    */
    }
    //-----------------------------------------------------------------------------------
    void        calculate_direction_()
    {
        if (_kbhit())
        {
            switch (_getch())
            {
            case 'w':
                direction = UP;
                break;
            case 'a':
                direction = LEFT;
                break;
            case 's':
                direction = DOWN;
                break;
            case 'd':
                direction = RIGHT;
                break;
            default:
                break;
            } // switch
        } // if
    }
    /*
    * функция отвественная за управление кнопками
    */
    //-----------------------------------------------------------------------------------
    bool        check_conflict_with_wall_() const
    {
        return      snake_.get_head_position_().x == 0
            || snake_.get_head_position_().x == WIDTH - 1
            || snake_.get_head_position_().y == 0
            || snake_.get_head_position_().y == HEIGHT - 1;
    }
    /*
    * функция проверяющая за проверку столкновением в стену
    */
    //-----------------------------------------------------------------------------------
    bool        check_conflict_with_food_() const
    {
        return      snake_.get_head_position_() == food_.get_food_position_();
    }
    /*
    * функция отвественная за проверку на поедание еды
    */
    //-----------------------------------------------------------------------------------
    bool        check_conflict_with_tail_() const
    {
        if (size_snake_ == 0)
        {
            return false;
        }
        for
            (
                auto i = std::next(snake_.get_all_snake_().begin());
                i != snake_.get_all_snake_().end();
                ++i
                )
        {
            if (*i == snake_.get_head_position_())
            {
                return true;
            }
        }
        return false;
    }/*
    * функция отвественная за проверку на ситуации  с хвостом
    */
    //-----------------------------------------------------------------------------------
    void        change_direction_()
    {
        T_point             new_head = *snake_.get_all_snake_().begin();
        T_list_<T_point>&& new_snake(snake_.get_all_snake_());
        switch (direction)
        {
        case UP:
            new_head.y--;
            break;
        case LEFT:
            new_head.x--;
            break;
        case DOWN:
            new_head.y++;
            break;
        case RIGHT:
            new_head.x++;
            break;
        default:
            break;
        }
        new_snake.push_front(new_head);
        T_point             erase_elem = *std::prev(new_snake.end());
        new_snake.erase(std::prev(new_snake.end()));
        snake_.move_new_snake_(std::move(new_snake));
        drawer_base_.draw_char_in_coord_(erase_elem);
        drawer_snake_.draw_snake_(snake_);
    }
    /*
    * функция отвественная за смену координат для головы змеи
    * new_snake.erase(std::prev(new_snake.end())); -  функ.  удаляет старую змейку 
     *snake_.move_new_snake_(std::move(new_snake));- функ. передвигает координату змейку
     *drawer_base_.draw_char_in_coord_(erase_elem);- функ.  помещающая  змейку в координаты
      *  drawer_snake_.draw_snake_(snake_); - функ.  рисуящая  новую  змейку 
    */
    //-----------------------------------------------------------------------------------
public:
    //-----------------------------------------------------------------------------------
    void        game_loop_()
    {
        initial_varialbes_and_draw_();
        for (;;)
        {
            calculate_direction_();
            if
                (
                    check_conflict_with_wall_()
                    //|| check_conflict_with_tail_()
                    )
            {
                drawer_base_.draw_char_in_coord_({ 0,0 }, '#');
                break;
            }
            if (check_conflict_with_food_())
            {
                food_.generate_new_position_food_();
                snake_.add_tail_snake_();
            }
            drawer_base_.draw_char_in_coord_(food_.get_food_position_(), '@');
            change_direction_();
            Sleep(50);
        }
    }
    /*
    * функция отвественная за игровый цикл
    */
    //-----------------------------------------------------------------------------------
};
/////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    T_snake_game game;
    game.game_loop_();
}
/*
* функция вызывающая саму игру
*/
