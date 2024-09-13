/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moassi <moassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 13:26:11 by moassi            #+#    #+#             */
/*   Updated: 2024/08/06 16:46:49 by moassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_HPP
#define MAIN_HPP

#include "Server.hpp"

/* ------------------------------INPUT VALIDATION-----------------------------*/

void validateArgs(int ac, char **av, int &port, std::string &password);

#endif  