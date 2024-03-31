using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CrateBehavior : MonoBehaviour
{
    private List<Collider>[] positionalArray = {new(), new(), new(), new()};
    private bool hasMoved = false;

    public void UpdatePositionalArray(string tag, Collider tile) {
        switch (tag) {
            case "North":
                positionalArray[0].Add(tile);
                break;
            case "East":
                positionalArray[1].Add(tile);
                break;
            case "South":
                positionalArray[2].Add(tile);
                break;
            case "West":
                positionalArray[3].Add(tile);
                break;
        }
    }

    public void RemoveFromPositionalArray(string tag, Collider tile) {
        switch (tag) {
            case "North":
                positionalArray[0].Remove(tile);
                break;
            case "East":
                positionalArray[1].Remove(tile);
                break;
            case "South":
                positionalArray[2].Remove(tile);
                break;
            case "West":
                positionalArray[3].Remove(tile);
                break;
        }
    }

    public List<Collider> GetPositionalArray(string tag) {
        return tag switch {
            "North" => positionalArray[0],
            "East" => positionalArray[1],
            "South" => positionalArray[2],
            "West" => positionalArray[3],
            _ => null,
        };
    }

    public bool ValidMovement(int dir) {
        bool crateValue = true;
        //Returns false if there is an obstacle in the way of the direction moved
        //North = 0, East = 1, South = 2, West = 3
        for (int i = 0; i < positionalArray[dir].Count; i++) {
            if (positionalArray[dir] == null) {
                crateValue &= true;
            } else if (positionalArray[dir][i].CompareTag("Wall") || positionalArray[dir][i].CompareTag("Player") || positionalArray[dir][i].CompareTag("Drone")) {
                crateValue &= false;
            } else if (positionalArray[dir][i].CompareTag("Crate")) {
                crateValue &= positionalArray[dir][i].GetComponent<CrateBehavior>().ValidMovement(dir);
            }
        }
        if (crateValue && !hasMoved) {
            hasMoved = true;
            transform.Translate(new Vector3(2 * ((dir == 1) ? 1 : ((dir == 3) ? -1 : 0)), 0, 2 * ((dir == 0) ? 1 : ((dir == 2) ? -1 : 0))));
        }
        return crateValue;
    }

    public void FinishedMove(int dir) {
        for (int i = 0; i < positionalArray[dir].Count; i++) {
            if (positionalArray[dir][i].CompareTag("Crate")) {
                positionalArray[dir][i].GetComponent<CrateBehavior>().FinishedMove(dir);
            }
        }
        hasMoved = false;
    }
}
