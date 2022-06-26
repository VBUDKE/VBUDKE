#include <list>
#include <iostream>
#include <random>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <chrono>
/////////////////////////////////////////////////////////////////////////////////////////
const int   WIDTH = 80;
const int   HEIGHT = 20;
/////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
using   T_list_ = std::list<T>;
/////////////////////////////////////////////////////////////////////////////////////////
struct T_point
{
    //-----------------------------------------------------------------------------------
    int  x;
    int  y;
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
    //-----------------------------------------------------------------------------------
    T_point operator+=  (const T_point& point)
    {
        x += point.x;
        y += point.y;
        return *this;
    }
    //-----------------------------------------------------------------------------------
    T_point operator+   (const T_point& point)   const
    {
        T_point res = *this;
        return  res += point;
    }
    //-----------------------------------------------------------------------------------
    bool    operator==  (const T_point& point)   const
    {
        return  x == point.x
            && y == point.y;
    }
    //-----------------------------------------------------------------------------------
    void    print_position_()                    const
    {
        std::cout
            << x << ' '
            << y << ' '
            << std::endl;
    }
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
    {
        snake.push_back(start_position);
    }
    //-----------------------------------------------------------------------------------
    T_point             get_head_position_()        const
    {
        return  *snake.begin();
    }
    //-----------------------------------------------------------------------------------
    T_point             get_end_tail_position_()    const
    {
        return *(std::prev(snake.end()));
    }
    //-----------------------------------------------------------------------------------
    T_list_<T_point>    get_all_snake_()            const
    {
        return  snake;
    }
    //-----------------------------------------------------------------------------------
    void                add_tail_snake_()
    {
        snake.push_back(*std::prev(snake.cend()));
    }
    //-----------------------------------------------------------------------------------
    void                move_new_snake_(T_list_<T_point>&& new_list)
    {
        snake = std::move(new_list);
    }
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
        static  std::uniform_int_distribution<> distance_width(1, WIDTH - 2);
        static  std::uniform_int_distribution<> distance_height(1, HEIGHT - 2);
        position_food.x = distance_width(gen);
        position_food.y = distance_height(gen);
    }
    //-----------------------------------------------------------------------------------
    T_point      get_food_position_()            const
    {
        return  position_food;
    }
    //-----------------------------------------------------------------------------------
};
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
    //-----------------------------------------------------------------------------------
    virtual     ~T_draw_base()
    {}
    //-----------------------------------------------------------------------------------
    void        draw_char_in_coord_(const T_point& point, const char& c = ' ')
    {
        coord_ = point;
        set_windows_coord_();
        SetConsoleCursorPosition(handle_, windows_coord_);
        std::cout.put(c);
        clear_windows_coord_();
    }
    //-----------------------------------------------------------------------------------
private:
    //-----------------------------------------------------------------------------------
    void        clear_windows_coord_()
    {
        windows_coord_.X = 0;
        windows_coord_.Y = 0;
    }
    //-----------------------------------------------------------------------------------
    void        set_windows_coord_()
    {
        windows_coord_.X = coord_.x;
        windows_coord_.Y = coord_.y;
    }
    //-----------------------------------------------------------------------------------
};
/////////////////////////////////////////////////////////////////////////////////////////
const HANDLE T_draw_base::handle_ = GetStdHandle(STD_OUTPUT_HANDLE);
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
    //-----------------------------------------------------------------------------------
public:
    //-----------------------------------------------------------------------------------
    T_snake_game
    (
    )
        :
        snake_({ 40,10 }),
        size_snake_(0)
    {}
    //-----------------------------------------------------------------------------------
private:
    //-----------------------------------------------------------------------------------
    void        initial_varialbes_and_draw_()
    {
        drawer_field_.draw_field_();
        food_.generate_new_position_food_();
        drawer_base_.draw_char_in_coord_(food_.get_food_position_(), '@');
        drawer_snake_.draw_snake_(snake_);
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
    //-----------------------------------------------------------------------------------
    bool        check_conflict_with_wall_() const
    {
        return      snake_.get_head_position_().x == 0
            || snake_.get_head_position_().x == WIDTH - 1
            || snake_.get_head_position_().y == 0
            || snake_.get_head_position_().y == HEIGHT - 1;
    }
    //-----------------------------------------------------------------------------------
    bool        check_conflict_with_food_() const
    {
        return      snake_.get_head_position_() == food_.get_food_position_();
    }
    //-----------------------------------------------------------------------------------
    //TODO
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
    }
    //-----------------------------------------------------------------------------------
    void        change_direction_()
    {
        //создаем новую змейку в которую пишем новые координаты змейки
        //потом перемещаем новую змейку вместо старой
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
    //-----------------------------------------------------------------------------------
};
/////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    T_snake_game game;
    game.game_loop_();
}
