//
// Created by root on 4/24/18.
//

#ifndef FEATHER_UTIL_HPP
#define FEATHER_UTIL_HPP

#include "inja/json.hpp"

namespace feather{

    template<typename T, typename = std::enable_if_t <iguana::is_reflection_v<T>>>
    inline nlohmann::json struct_to_json(T&& t){
        nlohmann::json val;
        iguana::for_each(std::forward<T>(t), [&t, &val](const auto& v, auto i){
            auto name = iguana::get_name<T>(decltype(i)::value);
            val[name.data()] = t.*v;
        });
        return val;
    }

    template<typename T, typename = std::enable_if_t <iguana::is_reflection_v<T>>>
    inline nlohmann::json struct_to_json(const std::vector<T>& v){
        nlohmann::json list;
        for(auto& t : v){
            iguana::for_each(t, [&t, &list](const auto& v, auto i){
                auto name = iguana::get_name<T>(decltype(i)::value);
                nlohmann::json val;
                val[name.data()] = t.*v;
                list.push_back(val);
            });
        }

        return list;
    }

    inline std::string render(std::string tpl_filepath, nlohmann::json data)
    {
        using namespace inja;
        Environment env = Environment("./www");
        env.set_element_notation(ElementNotation::Dot);
        Template temp = env.parse_template(tpl_filepath);
        std::string result = env.render_template(temp, data);
        return result;
    }
}

#endif //FEATHER_UTIL_HPP
