#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <ctime>
#include "nlohmann/json.hpp"

namespace Actor_json {
    struct Actor {
        std::string name;
        std::string surname;
        std::string country;
        std::tm birthday;
    };

    void to_json(nlohmann::json &j, const Actor &a) {
        char mbstr[100];
        std::strftime(mbstr, sizeof(mbstr), "%d %b %Y", &a.birthday);
        j = nlohmann::json{{"name",     a.name},
                           {"surname",  a.surname},
                           {"country",  a.country},
                           {"birthday", mbstr}};
    }

    void from_json(const nlohmann::json &j, Actor &a) {
        j.at("name").get_to(a.name);
        j.at("surname").get_to(a.surname);
        j.at("country").get_to(a.country);
        std::tm t = {};
        std::string ts;

        j.at("birthday").get_to(ts);

        std::istringstream ss(ts);
        ss >> std::get_time(&a.birthday, "%d %b %Y");
    }
}

namespace Cast_json {
    struct Cast {
        std::string character;
        Actor_json::Actor actor;
    };

    void to_json(nlohmann::json &j, const Cast &c) {
        j = nlohmann::json{{"character", c.character},
                           {"actor",     c.actor}};
    }

    void from_json(const nlohmann::json &j, Cast &c) {
        j.at("character").get_to(c.character);
        j.at("actor").get_to(c.actor);
    }
}

namespace Film_json {
    struct Film {
        std::string title;
        int year;
        int runtimeMin;
        std::string director;
        std::string cinematography;

        std::vector<std::string> productionCompanies;
        std::tm released;
        std::vector<Cast_json::Cast> casts;
    };

    void to_json(nlohmann::json &j, const Film &f) {
        j = nlohmann::json{{"title",          f.title},
                           {"year",           f.year},
                           {"runtimeMin",     f.runtimeMin},
                           {"director",       f.director},
                           {"cinematography", f.cinematography}};
        for (int i = 0; i < f.productionCompanies.size(); ++i) {
            j["productionCompanies"].push_back(f.productionCompanies[i]);
        };
        char mbstr[100];
        std::strftime(mbstr, sizeof(mbstr), "%d %b %Y", &f.released);
        j["released"] = mbstr;
        for (int i = 0; i < f.casts.size(); ++i) {
            nlohmann::json cast;
            cast = f.casts[i];
            j["casts"].push_back(cast);
        }
    }

    void from_json(const nlohmann::json &j, Film &f) {
        j.at("title").get_to(f.title);
        j.at("year").get_to(f.year);
        j.at("runtimeMin").get_to(f.runtimeMin);
        j.at("director").get_to(f.director);
        j.at("cinematography").get_to(f.cinematography);

        for (int i = 0; i < j.at("productionCompanies").size(); ++i) {
            std::string productionCompanies;
            j.at("productionCompanies")[i].get_to(productionCompanies);
            f.productionCompanies.push_back(productionCompanies);
        }

        for (int i = 0; i < j.at("casts").size(); ++i) {
            Cast_json::Cast cast;
            j.at("casts")[i].get_to(cast);
            f.casts.push_back(cast);
        }

        std::tm t = {};
        std::string ts;
        j.at("released").get_to(ts);
        std::istringstream ss(ts);
        ss >> std::get_time(&f.released, "%d %b %Y");
    }
}

struct IndexFilmCast {
    int index2Film;
    int index2Cast;
};

int main() {
    {//actors
        Actor_json::Actor Thomas{"Thomas", "Holland", "London, England", std::tm{0, 0, 0, 1, 5, 96}};
        Actor_json::Actor Zendaya{"Zendaya", "Coleman", "Oakland, California, U.S.", std::tm{0, 0, 0, 1, 8, 96}};
        Actor_json::Actor Benedict{"Benedict", "Cumberbatch", "Hammersmith, London, England",
                                   std::tm{0, 0, 0, 19, 6, 76}};
        Actor_json::Actor Mark{"Mark", "Wahlberg", "Boston, Massachusetts, U.S.", std::tm{0, 0, 0, 5, 5, 71}};
        Actor_json::Actor Sophia{"Sophia", "Ali", "San Diego, California, U.S.", std::tm{0, 0, 0, 7, 10, 95}};
        Actor_json::Actor Jenna{"Jenna", "Clause", "Buffalo, New York", std::tm{0, 0, 0, 20, 1, 99}};
        Actor_json::Actor Reign{"Reign", "Edwards", "Harford County, Maryland, U.S.", std::tm{0, 0, 0, 1, 11, 96}};
        Actor_json::Actor Timothee{"Timothee", "Chalamet", "New York City, U.S.", std::tm{0, 0, 0, 27, 11, 95}};
        Actor_json::Actor Rebecca{"Rebecca", "Ferguson", "Stockholm, Sweden", std::tm{0, 0, 0, 19, 9, 83}};
        Actor_json::Actor Oscar{"Benedict", "Isaac", "Guatemala City, Guatemala", std::tm{0, 0, 0, 9, 2, 79}};
        Actor_json::Actor Ben{"Ben", "Affleck", "Berkeley, California, U.S", std::tm{0, 0, 0, 15, 7, 72}};
        Actor_json::Actor Charlie{"Charlie", "Hunnam", "Newcastle upon Tyne, Tyne and Wear, England",
                                  std::tm{0, 0, 0, 10, 3, 80}};

        //1 Spider-Man: No Way Home
        Film_json::Film SpiderMan{"Spider-Man: No Way Home", 2021, 148, "Jon Watts", "Mauro Fiore"};
        SpiderMan.productionCompanies.push_back("Columbia Pictures");
        SpiderMan.productionCompanies.push_back("Marvel Studios");
        SpiderMan.productionCompanies.push_back("Pascal Pictures");
        SpiderMan.released = std::tm{0, 0, 0, 17, 11, 121};;
        SpiderMan.casts.push_back(Cast_json::Cast{"Peter Parker", Thomas});
        SpiderMan.casts.push_back(Cast_json::Cast{"MJ", Zendaya});
        SpiderMan.casts.push_back(Cast_json::Cast{"Doctor Strange", Benedict});

        //2 Uncharted
        Film_json::Film Uncharted{"Uncharted", 2022, 116, "Ruben Fleischer", "Chung-hoon Chung"};
        Uncharted.productionCompanies.push_back("Columbia Pictures");
        Uncharted.productionCompanies.push_back("Arad Productions");
        Uncharted.productionCompanies.push_back("Atlas Entertainment");
        Uncharted.productionCompanies.push_back("PlayStation Productions");
        Uncharted.released = std::tm{0, 0, 0, 18, 1, 122};;
        Uncharted.casts.push_back(Cast_json::Cast{"Nathan Drake", Thomas});
        Uncharted.casts.push_back(Cast_json::Cast{"Victor Sullivan", Mark});
        Uncharted.casts.push_back(Cast_json::Cast{"Chloe Frazer", Sophia});

        //3 The Wilds
        Film_json::Film Wilds{"The Wilds", 2020, 61, "Sarah Streicher", "Ed Wild"};
        Wilds.productionCompanies.push_back("Dylan Clark Productions");
        Wilds.productionCompanies.push_back("Fanfare Productions");
        Wilds.productionCompanies.push_back("ABC Signature");
        Wilds.productionCompanies.push_back("Amazon Studios");
        Wilds.productionCompanies.push_back("Baby Productions");
        Wilds.released = std::tm{0, 0, 0, 11, 11, 120};
        Wilds.casts.push_back(Cast_json::Cast{"Fatin Jadmani", Sophia});
        Wilds.casts.push_back(Cast_json::Cast{"Martha Blackburn", Jenna});
        Wilds.casts.push_back(Cast_json::Cast{"Rachel Reid", Reign});

        //4 Dune
        Film_json::Film Dune{"Dune", 2021, 156, "Denis Villeneuve", "Greig Fraser"};
        Dune.productionCompanies.push_back("Legendary Pictures");
        Dune.released = std::tm{0, 0, 0, 21, 9, 121};
        Dune.casts.push_back(Cast_json::Cast{"Paul Atreides", Timothee});
        Dune.casts.push_back(Cast_json::Cast{"Lady Jessica Atreides", Rebecca});
        Dune.casts.push_back(Cast_json::Cast{"Duke Leto Atreides", Oscar});

        //5 Triple
        Film_json::Film Triple{"Triple Frontier", 2019, 125, "J. C. Chandor", "Roman Vasyanov"};
        Triple.productionCompanies.push_back("Atlas Entertainment");
        Triple.productionCompanies.push_back("Netflix");
        Triple.released = std::tm{0, 0, 0, 6, 2, 119};

        Triple.casts.push_back(Cast_json::Cast{"Tom 'Redfly' Davis", Ben});
        Triple.casts.push_back(Cast_json::Cast{"Santiago 'Pope' Garcia", Oscar});
        Triple.casts.push_back(Cast_json::Cast{"William 'Ironhead' Miller", Charlie});

        nlohmann::json j;
        j["Spider-Man: No Way Home"] = SpiderMan;
        j["Uncharted"] = Uncharted;
        j["The Wilds"] = Wilds;
        j["Dune"] = Dune;
        j["Triple Frontier"] = Triple;

        std::ofstream f_out("films.json");
        f_out << j;
        f_out.close();
    }

    nlohmann::json j_in;
    std::ifstream f_in("films.json");
    f_in >> j_in;
    f_in.close();

    std::vector<Film_json::Film> films;
    std::map<std::string, std::vector<IndexFilmCast>> mapActors;

    for (auto &element: j_in) {
        Film_json::Film film;
        element.get_to(film);
        int index2Film = films.size();
        for (int i = 0; i < film.casts.size(); ++i) {
            mapActors[film.casts[i].actor.name].push_back(IndexFilmCast{index2Film, i});
        }
        films.push_back(film);
    }

    std::cout << "Actors name:" << std::endl;
    for (auto &key: mapActors) {
        std::cout << key.first << std::endl;
    }

    std::cout << "input name: ";
    std::string name;
    std::cin >> name;

    if (auto pos = mapActors.find(name); pos != mapActors.end()) {
        for (int i = 0; i < pos->second.size(); ++i) {
            std::cout << "Title: " << films[pos->second[i].index2Film].title << " ("
                      << films[pos->second[i].index2Film].year << ")" << std::endl;
            std::cout << "Actor: " << films[pos->second[i].index2Film].casts[pos->second[i].index2Cast].actor.name;
            std::cout << " " << films[pos->second[i].index2Film].casts[pos->second[i].index2Cast].actor.surname
                      << std::endl;
            std::cout << "Character: " << films[pos->second[i].index2Film].casts[pos->second[i].index2Cast].character
                      << std::endl;
            std::cout << "" << std::endl;
        }
    } else {
        std::cout << "Not found!";
    }

    return 0;
}
