CREATE DATABASE irrigacao;

USE irrigacao;

CREATE TABLE leituras (
    id INT AUTO_INCREMENT PRIMARY KEY,
    umidade INT,
    nivel INT,
    data_hora TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
